#include "Meteorite.h"

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

	transform->set_translate(pos);

	velocity_ = { -2, 0, 0 };
	//transform->set_translate_y(5.0f);
	//transform->set_translate_x(10.0f);
	//transform->set_translate_z(RandomFloat(-4.0f, 4.0f));

	isDead_ = false;
	isAttraction_ = false;
}

void Meteorite::Update() {
	transform->set_scale({ radius_, radius_, radius_ });
	Move();
}

void Meteorite::Move() {
	Vector3 translate = transform->get_translate();
	translate += velocity_ * (1.0f / 60) * kSpeed_;

	if (isAttraction_) {
		velocity_ += acceleration_ * (1.0f / 60) * attractionedStrength_;
	}


	if (translate.x < -10 || translate.x > 10) {
		isDead_ = true;
	}

	if (translate.z < -10 || translate.z > 10) {
		isDead_ = true;
	}

	transform->set_translate(translate);
}

#ifdef _DEBUG
#include <externals/imgui/imgui.h>
void Meteorite::EditImGui() {
}
#endif // _DEBUG