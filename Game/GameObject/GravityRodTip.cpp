#include "GravityRodTip.h"

float GravityRodTip::radius_ = 1.0f;

GravityRodTip::GravityRodTip(GameObject* gameObject) {
	Init(gameObject);
}

GravityRodTip::~GravityRodTip() {
}

void GravityRodTip::Init(GameObject* gameObject) {
	reset_object("particle.obj");
	set_parent(gameObject->get_hierarchy());

}

void GravityRodTip::Update(const float& radius) {
	transform->set_translate(Vector3{ 0, 0,radius});
	transform->set_scale({ radius_, radius_, radius_ });
}
