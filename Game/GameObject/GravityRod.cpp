#include "GravityRod.h"

GravityRod::GravityRod(GameObject* gameObject) {
	Init(gameObject);
}

GravityRod::~GravityRod() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GravityRod::Init(GameObject* gameObject) {
	reset_object("GravityRod.obj");

	set_parent(*gameObject);

	tipObject_[0] = std::make_unique<GravityRodTip>(this);
	tipObject_[1] = std::make_unique<GravityRodTip>(this);

	tipObject_[0]->reset_object("particle.obj");
	tipObject_[1]->reset_object("particle.obj");

	transform->set_translate(Vector3{ 0, 2.5f, 1.0f });
	transform->set_rotate(Quaternion::EulerDegree(0, 90, 0));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GravityRod::Update() {
	// 両方の先端にオブジェクトを配置する
	tipObject_[0]->Update(radius_);
	tipObject_[1]->Update(-radius_);

	// rodのベクトルを求める
	rodVector_ = Vector3::Normalize(tipObject_[0]->get_transform().get_translate() - tipObject_[1]->get_transform().get_translate());
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
	ImGui::DragFloat("Range", &attractionRange_, 0.1f);
	ImGui::DragFloat("TipRadius", &GravityRodTip::radius_, 0.1f);

	ImGui::Separator();
	debug_gui();
	ImGui::End();
}
#endif // _DEBUG