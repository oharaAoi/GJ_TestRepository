#include "Player.h"
#include "Engine/Game/Transform3D/Transform3D.h"
#include <externals/imgui/imgui.h>

Player::Player() {
	Init();
}

Player::~Player() {
}

void Player::Init() {
	reset_object("player.obj");

	gravityRod_ = std::make_unique<GravityRod>(this);
	isAttack_ = false;

	input_ = Input::GetInstance();
	Vector3 translate = transform->get_translate();
	transform->set_translate_y(12.5f);
}

void Player::Update(const float& fieldRadius) {
	moveRotation = CQuaternion::IDENTITY;
	Move(fieldRadius);
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

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- 移動を行う関数 ------------------- //
void Player::Move(const float& fieldRadius) {
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
		// 中心からのベクトル
		Vector3 distance = translate - Vector3(0, translate.y, 0);
		// 中心からの長さ
		float lenght = Vector3::Length(translate, Vector3(0, translate.y, 0));
		if (lenght > fieldRadius) {
			distance = Vector3::Normalize(distance) * fieldRadius;
			translate = {distance.x, translate.y, distance.z};
		}

		transform->set_translate(translate);

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

const Quaternion& Player::GetMoveQuaternion() const {
	return moveRotation;
}
