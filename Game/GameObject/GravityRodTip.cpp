#include "GravityRodTip.h"

GravityRodTip::GravityRodTip() {
}

GravityRodTip::~GravityRodTip() {
}

void GravityRodTip::Init() {
	reset_object("particle.obj");
}

void GravityRodTip::Update(const float& radius, const Quaternion& rodQuaternion, const Vector3& translation) {
	Vector3 forword = Vector3{ 0,0,1 } * rodQuaternion;
	transform->set_translate(translation + (forword * radius));
}
