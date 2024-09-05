#include "GameScene.h"

#include "Engine/Game/Managers/SceneManager/SceneManager.h"
#include "Engine/Utility/Utility.h"
#include "Engine/Render/RenderPathManager/RenderPathManager.h"
#include "Engine/DirectX/DirectXCore.h"

void GameScene::initialize() {
	Input::GetInstance()->Init(WinApp::GetWNDCLASS(), WinApp::GetWndHandle());

	field_ = std::make_unique<Field>();
	player_ = std::make_unique<Player>();

	camera3D_ = std::make_unique<FollowCamera>();
	camera3D_->initialize();
	camera3D_->set_transform({
		CVector3::BASIS,
		Quaternion::EulerDegree(90, 0, 0),
		{ 0, 35, 0 }
	});
}

void GameScene::begin() {
}

void GameScene::update() {

	// -------------------------------------------------
	// ↓ Inputの更新
	// -------------------------------------------------
	Input::GetInstance()->Update();

	// -------------------------------------------------
	// ↓ カメラを更新
	// -------------------------------------------------
	camera3D_->update();
	camera3D_->update_matrix();

	// -------------------------------------------------
	// ↓ GameObjectの更新
	// -------------------------------------------------
	player_->Update();

	frameCount_++;

	if (frameCount_ > 120) {
		AddMeteoriteList();
		frameCount_ = 0;
	}

	for (Meteorite& meteo : meteoriteList_) {
		meteo.Update();
	}
}

void GameScene::begin_rendering() {
	field_->begin_rendering(*camera3D_);
	
	player_->Begin_Rendering(camera3D_.get());

	for (Meteorite& meteo : meteoriteList_) {
		meteo.begin_rendering(*camera3D_);
	}
}

void GameScene::late_update() {

}

void GameScene::draw() const {
	RenderPathManager::BeginFrame();
	DirectXCore::ShowGrid(*camera3D_);
	field_->draw();
	player_->Draw();

	for (const Meteorite& meteo : meteoriteList_) {
		meteo.draw();
	}

	RenderPathManager::Next();
}

void GameScene::AddMeteoriteList() {
	meteoriteList_.emplace_back();
}

#ifdef _DEBUG

#include <externals/imgui/imgui.h>
void GameScene::debug_update() {
	ImGui::Begin("GameScene");
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

	field_->EditImGui();
	ImGui::Begin("GameObject");
	player_->debug_gui();
	ImGui::End();
}
#endif // _DEBUG
