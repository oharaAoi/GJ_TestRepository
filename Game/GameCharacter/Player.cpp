#include "Player.h"
#include "Engine/Game/Transform3D/Transform3D.h"

Player::Player() {
	Init();
}

Player::~Player() {
}

void Player::Init() {
	reset_object("player.obj");

	gravityRod_ = std::make_unique<GravityRod>();
	isAttack_ = false;

	input_ = Input::GetInstance();
	Vector3 translate = transform->get_translate();
	transform->set_translate_y(4.5f);
}

void Player::Update() {
	moveRotation = CQuaternion::IDENTITY;
	Move();
	Attack();

	if (isAttack_) {
		gravityRod_->Update(transform->get_translate(), forwordRotation);
	}

	gravityRod_->EditImGui();
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
		velocity = velocity.normalize() * speed * (1.0f / 60);
		translate.x += velocity.x;
		translate.z += velocity.y;
		transform->set_translate(translate);

		// playerの向きを移動方向にする
		float targetAngle = std::atan2f(velocity.x, velocity.y);
		moveRotation = Quaternion::EulerRadian({0,targetAngle,0});
		playerQuaternion = moveRotation;
		forwordRotation = moveRotation;
		transform->set_rotate(playerQuaternion);
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
