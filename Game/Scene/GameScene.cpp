#include "GameScene.h"

#include "Engine/Game/Managers/SceneManager/SceneManager.h"
#include "Engine/Utility/Utility.h"
#include "Engine/Render/RenderPathManager/RenderPathManager.h"
#include "Engine/DirectX/DirectXCore.h"

void GameScene::initialize() {
	planet_ = std::make_unique<Planet>();

	player_ = std::make_unique<Player>();

	camera3D_ = std::make_unique<Camera3D>();
	camera3D_->initialize();
	camera3D_->set_transform({
		CVector3::BASIS,
		Quaternion::EulerDegree(45, 0, 0),
		{ 0, 10, -10 }
							 });
}

void GameScene::begin() {
}

void GameScene::update() {
	camera3D_->update_matrix();
	camera3D_->update();

	player_->Update();
}

void GameScene::begin_rendering() {
	//planet_->begin_rendering(*camera3D_);
	player_->begin_rendering(*camera3D_);
}

void GameScene::late_update() {

}

void GameScene::draw() const {
	RenderPathManager::BeginFrame();
	DirectXCore::ShowGrid(*camera3D_);
	planet_->draw();
	player_->draw();
	RenderPathManager::Next();
}


#ifdef _DEBUG

#include <externals/imgui/imgui.h>
void GameScene::debug_update() {
	ImGui::Begin("DemoScene");
	if (ImGui::Button("StackScene")) {
		SceneManager::SetSceneChange(CreateUnique<GameScene>(), true);
	}
	if (ImGui::Button("ChangeScene")) {
		SceneManager::SetSceneChange(CreateUnique<GameScene>(), false);
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
