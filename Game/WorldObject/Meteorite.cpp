#include "Meteorite.h"
#include "Game/Enviroment.h"

float Meteorite::attractionedStrength_ = 3;
float Meteorite::kSpeed_ = 2.0f;
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
	//transform->set_translate_y(5.0f);
	//transform->set_translate_x(10.0f);
	//transform->set_translate_z(RandomFloat(-4.0f, 4.0f));

	isDead_ = false;
	isAttraction_ = false;
	isFalling_ = false;
}

void Meteorite::Update() {
	if (!isFalling_) {
		Move();
		isAttraction_ = false;
	} else {
		Falling();
	}
}

void Meteorite::Move() {
	velocity_.y = 0;
	acceleration_.y = 0;
	Vector3 translate = transform->get_translate();
	translate += velocity_ * kDeltaTime * kSpeed_;
	// 引き寄せられている間の処理
	if (isAttraction_) {
		velocity_ += acceleration_ * kDeltaTime * attractionedStrength_;
	}

	// 範囲外に出たら削除する処理
	if (std::abs(translate.x) > 20 || std::abs(translate.y) > 20 || std::abs(translate.z) > 20) {
		isDead_ = true;
	}

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