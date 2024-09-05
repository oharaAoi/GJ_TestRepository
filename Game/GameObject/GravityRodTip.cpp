#include "GravityRodTip.h"

GravityRodTip::GravityRodTip() {
}

GravityRodTip::~GravityRodTip() {
}

void GravityRodTip::Init() {
	reset_object("particle.obj");
}

void GravityRodTip::Update(const float& radius, const Quaternion& rodQuaternion, const Vector3& translation) {
	transform->set_translate((rodQuaternion.vector() * radius) + translation);
}
