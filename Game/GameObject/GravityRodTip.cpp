#include "GravityRodTip.h"

float GravityRodTip::radius_ = 1.0f;

GravityRodTip::GravityRodTip() {
	Init();
}

GravityRodTip::~GravityRodTip() {
}

void GravityRodTip::Init() {
	reset_object("particle.obj");
}

void GravityRodTip::Update(const float& radius, const Quaternion& rodQuaternion, const Vector3& translation) {
	Vector3 forword = Vector3{ 0,0,1 } * rodQuaternion;
	transform->set_translate(translation + (forword * radius));
	transform->set_scale({ radius_, radius_, radius_ });
}
