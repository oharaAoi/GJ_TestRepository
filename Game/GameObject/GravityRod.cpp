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

	tipObject_[Tips::Tips_origine] = std::make_unique<GravityRodTip>(this);
	tipObject_[Tips::Tips_end] = std::make_unique<GravityRodTip>(this);

	tipObject_[Tips::Tips_origine]->reset_object("particle.obj");
	tipObject_[Tips::Tips_end]->reset_object("particle.obj");

	transform->set_translate(Vector3{ 0, 2.5f, 1.0f });
	transform->set_rotate(Quaternion::EulerDegree(0, 90, 0));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void GravityRod::Update() {
	// 両方の先端にオブジェクトを配置する
	tipObject_[Tips::Tips_origine]->Update(radius_);
	tipObject_[Tips::Tips_end]->Update(-radius_);

	// rodのベクトルを求める
	//rodVector_ = Vector3::Normalize(tipObject_[Tips::Tips_end]->world_position() - tipObject_[Tips::Tips_origine]->world_position());
}

void GravityRod::Begin_Rendering(Camera3D* camera3d) {
	begin_rendering(*camera3d);
	tipObject_[Tips::Tips_origine]->begin_rendering(*camera3d);
	tipObject_[Tips::Tips_end]->begin_rendering(*camera3d);
}

void GravityRod::Draw() const {
	draw();
	tipObject_[Tips::Tips_origine]->draw();
	tipObject_[Tips::Tips_end]->draw();
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