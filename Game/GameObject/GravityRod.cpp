#include "GravityRod.h"

GravityRod::GravityRod() {
	Init();
}

GravityRod::~GravityRod() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GravityRod::Init() {
	reset_object("GravityRod.obj");

	tipObject_[0] = std::make_unique<GravityRodTip>();
	tipObject_[1] = std::make_unique<GravityRodTip>();

	tipObject_[0]->reset_object("particle.obj");
	tipObject_[1]->reset_object("particle.obj");
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GravityRod::Update(const Vector3& playerPos, const Quaternion& quaternion) {
	transform->set_translate(playerPos);
	transform->set_rotate(quaternion);

	// 両方の先端にオブジェクトを配置する
	tipObject_[0]->Update(radius_, transform->get_quaternion(), transform->get_translate());
	tipObject_[1]->Update(-radius_, transform->get_quaternion(), transform->get_translate());
}

void GravityRod::Begin_Rendering(Camera3D* camera3d) {
	begin_rendering(*camera3d);
	tipObject_[0]->begin_rendering(*camera3d);
	tipObject_[1]->begin_rendering(*camera3d);
}

void GravityRod::Draw() const {
	draw();
	tipObject_[0]->draw();
	tipObject_[1]->draw();
}

#ifdef _DEBUG
#include <externals/imgui/imgui.h>
void GravityRod::EditImGui() {
	ImGui::Begin("GravityRod");
	ImGui::DragFloat("radius", &radius_, 0.1f);
	ImGui::End();
}
#endif // _DEBUG