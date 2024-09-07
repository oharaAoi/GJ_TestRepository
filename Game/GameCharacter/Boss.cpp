#include "Boss.h"
#include "Engine/Game/Transform3D/Transform3D.h"

Boss::Boss() {
	Init();
}

Boss::~Boss() {
}

void Boss::Init() {
	reset_object("mouth.obj");
	Vector3 translate = transform->get_translate();
	translate.y = -2.0f;
	transform->set_translate(translate);
}

void Boss::Update() {

}
