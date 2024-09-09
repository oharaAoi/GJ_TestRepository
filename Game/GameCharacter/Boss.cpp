#include "Boss.h"
#include "Engine/Game/Transform3D/Transform3D.h"
#include <Game/Enviroment.h>

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

	velocity_ = { 0,0.2f,0 };

	pushBackValue_ = 0.0f;
	pushBackStrength_ = 0.2f;

	debugObject_ = std::make_unique<GameObject>();
	debugObject_->reset_object("GravityRod.obj");
	debugObject_->set_parent(*hierarchy);
}

void Boss::Update() {
	Move();
}

void Boss::Begin_Rendering(Camera3D* camera3d) {
	begin_rendering(*camera3d);
	debugObject_->begin_rendering(*camera3d);
}

void Boss::Draw() const {
	draw();
	debugObject_->draw();
}

void Boss::Move() {
	Vector3 translate = transform->get_translate();
	translate += velocity_ * kDeltaTime;

	translate.y -= pushBackValue_;
	pushBackValue_ -= 0.1f;
	pushBackValue_ = std::max(0.0f, pushBackValue_);

	transform->set_translate(translate);
}

void Boss::OnCollision() {
	pushBackValue_ += pushBackStrength_;
}


#ifdef _DEBUG
#include "externals/imgui/imgui.h"
void Boss::EditImGui() {
	ImGui::Begin("Boss");
	ImGui::DragFloat("pushBackStrength", &pushBackStrength_, 0.1f, 0.0f, 1.0f);
	ImGui::Separator();
	debug_gui();
	ImGui::End();
}
#endif
