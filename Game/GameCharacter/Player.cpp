#include "Player.h"
#include "Engine/Game/Transform3D/Transform3D.h"
#include "Engine/Game/Color/Color.h"
#include <externals/imgui/imgui.h>
#include "Game/Enviroment.h"
#include "Game/Effect/EffectManager.h"


Player::Player() {
	Init();
}

Player::~Player() {
}

void Player::Init() {
	reset_object("playerBody.obj");

	effectManager_ = EffectManager::GetInstance();

	sphereCollider_ = std::make_unique<SphereCollider>();
	sphereCollider_->initialize();
	sphereCollider_->get_hierarchy().set_parent(this->get_hierarchy());
	sphereCollider_->set_on_collision(std::bind(&Player::On_Collision, this, std::placeholders::_1));
	sphereCollider_->set_on_collision_enter(std::bind(&Player::On_Collision_Enter, this, std::placeholders::_1, (&isAttackofEnmey_)));

	playerArm_ = std::make_unique<GameObject>();
	playerArm_->reset_object("playerArm.obj");
	playerArm_->set_parent(this->get_hierarchy());
	playerArm_->get_transform().set_translate_y(1.0f);

	gravityRod_ = std::make_unique<GravityRod>(this);
	isAttack_ = false;

	floatinGimmick_.parameter = 0;
	floatinGimmick_.period = 120;
	floatinGimmick_.amplitude = 0.01f;

	input_ = Input::GetInstance();
	Vector3 translate = transform->get_translate();
	transform->set_translate_y(12.5f);

	enemyKick_SE_ = std::make_unique<AudioPlayer>();
	enemyKick_SE_->initialize("SE_playerKick.wav", 0.5f, false);
}

void Player::Update(const float& fieldRadius) {
	fieldRadius_ = fieldRadius;

	// スタン中なら
	if (isStan_) {
		if (++stanFrame_ < stanTime_) {
			Vector3 translate = transform->get_translate();
			translate += preFrameVelocity_ * GameTimer::DeltaTime();
			ConstrainToField(translate);
			transform->set_translate(translate);
			transform->set_translate_y(12.5f);
			return;
		} else {
			isStan_ = false;
			stanFrame_ = 0;
			this->get_materials()[0].color = { 1,1.0f,1.0f,1 };
		}
	}
	
	Move();
	Attack();
	Floating();

	if (isAttack_) {
		gravityRod_->Update();

		//playerArm_->get_transform().set_rotate(Quaternion::EulerDegree(-70, 0, 0));
	}
}

void Player::Begin_Rendering(Camera3D* camera3d) {
	begin_rendering(*camera3d);
	playerArm_->begin_rendering(*camera3d);
	gravityRod_->Begin_Rendering(camera3d);
}

void Player::Draw() const {
	draw();
	playerArm_->draw();
	if (isAttack_) {
		gravityRod_->Draw();
		playerArm_->get_transform().set_rotate(Quaternion::Slerp(
			playerArm_->get_transform().get_quaternion(),
			Quaternion::EulerDegree(-120, 0, 0),
			0.3f
		));
	} else {
		playerArm_->get_transform().set_rotate(Quaternion::Slerp(
			playerArm_->get_transform().get_quaternion(),
			Quaternion::EulerDegree(0, 0, 0),
			0.3f
		));
	}
}

void Player::On_Collision(const BaseCollider* const other) {
	if (isAttackofEnmey_) {
		isAttack_ = false;
		isStan_ = true;
		this->get_materials()[0].color = { 1,0.0f,0.0f,1 };
	}
}

void Player::On_Collision_Enter(const BaseCollider* const other, bool* isEnemyAttack) {
	if (isAttackofEnmey_) {
		isAttack_ = false;
		isStan_ = true;
		this->get_materials()[0].color = { 1,0.0f,0.0f,1 };

		Vector3 direction = (other->world_position() - world_position()).normalize_safe();
		KnockBack(-direction);

	} else {
		enemyKick_SE_->restart();

		//effectManager_->AddEffect("syouma", world_position(), { 0,1,0 });

	}
}

void Player::On_Collision_Exit(const BaseCollider* const) {

}
#ifdef _DEBUG
void Player::Debug_Gui() {
	ImGui::Begin("Player");
	debug_gui();
	ImGui::End();
	ImGui::Begin("GravityRod");
	gravityRod_->EditImGui();
	playerArm_->debug_gui();
	ImGui::End();
}
#endif

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- 移動を行う関数 ------------------- //
void Player::Move() {
	if (isStan_) {
		return;
	}

	Vector3 translate = transform->get_translate();
	Quaternion playerQuaternion = transform->get_quaternion();

	if (input_->GetIsGamePadConnected(0)) {
		Vector2 velocity = input_->GetLeftJoyStick();
		velocity_ = { velocity.x, 0.0f, velocity.y };
		bool isMove = false;
		// 入力がさせていたら
		if (velocity.x != 0 || velocity.y) { isMove = true; }

		if (!isMove) {
			return;
		}
	
		// playerを動かす
		velocity = velocity * speed * GameTimer::DeltaTime();
		if (!isAttack_) {
			translate.x += velocity.x;
			translate.z += velocity.y;
		}

		// -------------------------------------------------
		// ↓ playerが円柱の面から出ない処理を行う
		// -------------------------------------------------
		ConstrainToField(translate);
		transform->set_translate(translate);
		transform->set_translate_y(12.5f);

		// playerの向きを移動方向にする
		float targetAngle = std::atan2f(velocity.x, velocity.y);
		moveRotation = Quaternion::EulerRadian({0,targetAngle,0});
		// 攻撃中は滑らかに移動
		if (isAttack_) {
			Quaternion slerp = Quaternion::Slerp(playerQuaternion, moveRotation, 0.05f);
			playerQuaternion = moveRotation;
			forwordRotation = slerp;
			transform->set_rotate(slerp);
		} else {
			forwordRotation = moveRotation;
			transform->set_rotate(moveRotation);
		}
	}
}

// ------------------- 攻撃を行う関数 ------------------- //
void Player::Attack() {
	if (input_->GetIsGamePadConnected(0)) {
		if (input_->GetIsPadTrigger(XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			isAttack_ = !isAttack_;
		}
	}
}

void Player::KnockBack(const Vector3& dire) {
	preFrameVelocity_ = -(dire * 4.0f);
}

void Player::ConstrainToField(Vector3& translate) {
	Vector3 distance = (translate - Vector3(0, translate.y, 0)).normalize_safe();
	// 中心からの長さ
	float lenght = Vector3::Length(translate, Vector3(0, translate.y, 0));
	if (lenght > fieldRadius_) {
		distance = distance * fieldRadius_;
		translate = { distance.x, translate.y, distance.z };
	}
}

void Player::Floating() {
	float upTranslate = transform->get_translate().y;
	// 口を動かすアニメーションを行う
	const float step = (2.0f * PI) / static_cast<float>(floatinGimmick_.period);
	floatinGimmick_.parameter += step;
	floatinGimmick_.parameter = std::fmod(floatinGimmick_.parameter, 2.0f * PI);
	// 移動させる量を動かす
	upTranslate += std::sin(floatinGimmick_.parameter) * 0.01f;
	transform->set_translate_y(upTranslate);
}

const Quaternion& Player::GetMoveQuaternion() const {
	return moveRotation;
}
