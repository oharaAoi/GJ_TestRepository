#include "Player.h"
#include "Engine/Game/Transform3D/Transform3D.h"

Player::Player() {
	Init();
}

Player::~Player() {
}

void Player::Init() {
	reset_object("player.obj");

	input_ = Input::GetInstance();
	Vector3 translate = transform->get_translate();
	transform->set_translate_y(10.0f);
}

void Player::Update(const Vector3& planetPos, const float& planetRadius) {
	moveRotation = CQuaternion::IDENTITY;
	Move(planetPos, planetRadius);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- 移動を行う関数 ------------------- //
void Player::Move(const Vector3& planetPos, const float& planetRadius) {
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
		velocity = velocity.normalize() * 0.1f;
		translate.x += velocity.x;
		translate.z += velocity.y;
		transform->set_translate(translate);

		// playerの向きを移動方向にする
		float targetAngle = std::atan2f(velocity.x, velocity.y);
		moveRotation = Quaternion::EulerRadian({0,targetAngle,0});
		playerQuaternion = moveRotation;
		transform->set_rotate(playerQuaternion);
	}
}

const Quaternion& Player::GetMoveQuaternion() const {
	return moveRotation;
}
