#include "Player.h"
#include "Engine/Game/Transform3D/Transform3D.h"
#include "Engine/Game/Color/Color.h"
#include <externals/imgui/imgui.h>
#include "Game/Enviroment.h"


Player::Player() {
	Init();
}

Player::~Player() {
}

void Player::Init() {
	reset_object("player.obj");

	sphereCollider_ = std::make_unique<SphereCollider>();
	sphereCollider_->initialize();
	sphereCollider_->get_hierarchy().set_parent(this->get_hierarchy());
	sphereCollider_->set_on_collision(std::bind(&Player::On_Collision, this, std::placeholders::_1));
	sphereCollider_->set_on_collision_enter(std::bind(&Player::On_Collision_Enter, this, std::placeholders::_1));

	gravityRod_ = std::make_unique<GravityRod>(this);
	isAttack_ = false;

	input_ = Input::GetInstance();
	Vector3 translate = transform->get_translate();
	transform->set_translate_y(12.5f);
}

void Player::Update(const float& fieldRadius) {
	fieldRadius_ = fieldRadius;

	if (isStan_) {
		if (++stanFrame_ < stanTime_) {
			Vector3 translate = transform->get_translate();
			translate += preFrameVelocity_ * kDeltaTime;
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
	
	moveRotation = CQuaternion::IDENTITY;
	Move();
	Attack();

	if (isAttack_) {
		gravityRod_->Update();
	}



#ifdef _DEBUG
	ImGui::Begin("GravityRod");
	gravityRod_->EditImGui();
	ImGui::End();
#endif
}

void Player::Begin_Rendering(Camera3D* camera3d) {
	begin_rendering(*camera3d);
	if (isAttack_) {
		gravityRod_->Begin_Rendering(camera3d);
	}
}

void Player::Draw() const {
	draw();
	if (isAttack_) {
		gravityRod_->Draw();
	}
}

void Player::On_Collision(const BaseCollider* const other) {
	if (isAttackofEnmey_) {
		isAttack_ = false;
		isStan_ = true;
		this->get_materials()[0].color = { 1,0.0f,0.0f,1 };
	}
}

void Player::On_Collision_Enter(const BaseCollider* const other) {
	if (isAttackofEnmey_) {
		isAttack_ = false;
		isStan_ = true;
		this->get_materials()[0].color = { 1,0.0f,0.0f,1 };

		Vector3 direction = (other->world_position() - world_position()).normalize_safe();
		KnockBack(direction);
	}

}

void Player::On_Collision_Exit(const BaseCollider* const) {

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- 移動を行う関数 ------------------- //
void Player::Move() {
	Vector3 translate = transform->get_translate();
	Quaternion playerQuaternion = transform->get_quaternion();

	if (input_->GetIsGamePadConnected(0)) {
		Vector2 velocity = input_->GetLeftJoyStick();
		bool isMove = false;
		// 入力がさせていたら
		if (velocity.x != 0 || velocity.y) { isMove = true; }

		if (!isMove) {
			return;
		}
	
		// playerを動かす
		velocity = velocity * speed * (1.0f / 60);
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

const Quaternion& Player::GetMoveQuaternion() const {
	return moveRotation;
}
