#include "Meteorite.h"
#include "Game/Enviroment.h"

float Meteorite::kAttractionedStrength_ = 100;
float Meteorite::kSpeed_ = 1.0f;
float Meteorite::radius_ = 1.0f;

Meteorite::Meteorite(const Vector3& pos) {
	Init(pos);
}

Meteorite::~Meteorite() {
}

void Meteorite::Init(const Vector3& pos) {
	reset_object("particle.obj");
	Vector3 position = pos;
	transform->set_translate(pos);
	transform->set_translate_y(14.0f);
	transform->set_scale({ radius_, radius_, radius_ });

	velocity_ = { -2, 0, 0 };
	
	attractionedStrength_ = kAttractionedStrength_;
	speed_ = kSpeed_;
	attractionRange_ = 3.0f;

	isDead_ = false;
	isAttraction_ = false;
	isFalling_ = false;
	isEnemyHit_ = false;
}

void Meteorite::Update(const Vector3& playerPosition) {
	attractionedStrength_ = kAttractionedStrength_;
	attractionRange_ = 3.0f;
	if (isEnemyHit_) {
		speed_ = 0.5f;
	}

	if (!isFalling_) {
		Move(playerPosition);
		isAttraction_ = false;
	} else {
		Falling();
	}
}

void Meteorite::Move(const Vector3& playerPosition) {
	velocity_.y = 0;
	acceleration_.y = 0;
	Vector3 translate = transform->get_translate();
	// 引き寄せられている間の処理
	if (isAttraction_) {
		acceleration_ += Vector3::Normalize(targetPosition_ - world_position()) * kDeltaTime;
		velocity_ = (acceleration_ * attractionedStrength_) * kDeltaTime;
	}

	// 範囲外に出たら削除する処理
	if (std::abs(translate.x) > 20 || std::abs(translate.y) > 20 || std::abs(translate.z) > 20) {
		isDead_ = true;
	}

	translate += (velocity_ * kSpeed_) * kDeltaTime;
	transform->set_translate(translate);
}

void Meteorite::Falling() {
	acceleration_ = { 0,kGravity_,0 };
	Vector3 translate = transform->get_translate();
	velocity_ += acceleration_ * kDeltaTime;
	translate += velocity_ * kDeltaTime;
	transform->set_translate(translate);
}

void Meteorite::OnCollision(const Vector3& other) {
	if (!isFalling_) {
		velocity_ += Vector3::Normalize(other - transform->get_translate()) * -2.0f;
	}
	isFalling_ = true;
}

#ifdef _DEBUG
#include <externals/imgui/imgui.h>
void Meteorite::EditImGui() {
}
#endif // _DEBUG