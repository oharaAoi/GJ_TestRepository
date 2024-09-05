#include "Meteorite.h"

float Meteorite::attractionedStrength_ = 3;

Meteorite::Meteorite() {
	Init();
}

Meteorite::~Meteorite() {
}

void Meteorite::Init() {
	reset_object("particle.obj");

	velocity_ = { -2, 0, 0 };
	transform->set_translate_y(4.5);
	transform->set_translate_x(10.0f);
	transform->set_translate_z(RandomFloat(-4.0f, 4.0f));
}

void Meteorite::Update() {
	Move();
}

void Meteorite::Move() {
	Vector3 translate = transform->get_translate();
	translate += velocity_ * (1.0f / 60) * speed;

	if (isAttraction_) {
		velocity_ += acceleration_ * (1.0f / 60) * attractionedStrength_;
	}

	transform->set_translate(translate);
}

#ifdef _DEBUG
#include <externals/imgui/imgui.h>
void Meteorite::EditImGui() {
}
#endif // _DEBUG