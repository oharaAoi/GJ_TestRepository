#include "Player.h"
#include "Engine/GameObject/Transform3D/Transform3D.h"

Player::Player() {
	Init();
}

Player::~Player() {
}

void Player::Init() {
}

void Player::Update() {
	Vector3 translate = transform->get_translate();
	if (Input::GetInstance()->GetKey(DIK_W)) {
		translate.z += 0.1f;
	}

	if (Input::GetInstance()->GetKey(DIK_S)) {
		translate.z -= 0.1f;
	}

	if (Input::GetInstance()->GetKey(DIK_A)) {
		translate.x -= 0.1f;
	}

	if (Input::GetInstance()->GetKey(DIK_D)) {
		translate.x += 0.1f;
	}

	transform->set_translate_z(translate.z);
	transform->set_translate_x(translate.x);
}
