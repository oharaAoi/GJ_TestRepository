#include "SceneDemo.h"

#include "Engine/Game/Managers/SceneManager/SceneManager.h"
#include "Engine/Utility/Utility.h"
#include "Engine/Render/RenderPathManager/RenderPathManager.h"
#include "Engine/DirectX/DirectXCore.h"

void SceneDemo::load() {
}

void SceneDemo::initialize() {
	planet_ = std::make_unique<Planet>();

	camera3D_ = std::make_unique<Camera3D>();
	camera3D_->initialize();
	camera3D_->set_transform({
		CVector3::BASIS,
		Quaternion::EulerDegree(45, 0, 0),
		{ 0, 10, -10 }
	});
}

void SceneDemo::finalize() {
}

void SceneDemo::begin() {
}

void SceneDemo::update() {
	camera3D_->update_matrix();
	camera3D_->update();
}

void SceneDemo::begin_rendering() {
	planet_->begin_rendering(*camera3D_);
}

void SceneDemo::late_update() {
}

void SceneDemo::draw() const {

	DirectXCore::ShowGrid(*camera3D_);
	planet_->draw();
}

#ifdef _DEBUG

#include <externals/imgui/imgui.h>
void SceneDemo::debug_update() {
	ImGui::Begin("DemoScene");
	if (ImGui::Button("StackScene")) {
		SceneManager::SetSceneChange(CreateUnique<SceneDemo>(), true);
	}
	if (ImGui::Button("ChangeScene")) {
		SceneManager::SetSceneChange(CreateUnique<SceneDemo>(), false);
	}
	if (ImGui::Button("PopScene")) {
		SceneManager::PopScene();
	}
	ImGui::End();

	ImGui::Begin("Camera3D");
	camera3D_->debug_gui();
	ImGui::End();
}
#endif // _DEBUG
