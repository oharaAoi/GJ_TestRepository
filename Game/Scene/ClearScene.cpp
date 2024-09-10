#include "ClearScene.h"
#include "Engine/Game/Managers/SceneManager/SceneManager.h"

void ClearScene::initialize() {
	input_ = Input::GetInstance();
	camera3D_ = std::make_unique<Camera3D>();
}

void ClearScene::load() {
}

void ClearScene::begin() {
}

void ClearScene::update() {
	// -------------------------------------------------
	// ↓ Inputの更新
	// -------------------------------------------------
	Input::GetInstance()->Update();


	if (input_->GetIsPadTrigger(XINPUT_GAMEPAD_A) || input_->GetKey(DIK_SPACE)) {
		SceneManager::SetSceneChange(CreateUnique<TitleScene>(), false);
	}
}

void ClearScene::begin_rendering() {
	camera3D_->update_matrix();
}

void ClearScene::late_update() {
}

void ClearScene::draw() const {
	RenderPathManager::BeginFrame();

	RenderPathManager::Next();
}

#ifdef _DEBUG
#include <externals/imgui/imgui.h>
void ClearScene::debug_update() {
	ImGui::Begin("Clear");
	ImGui::Text("nowScene: Clear");
	ImGui::Text("nextScene: Title");
	ImGui::Text("push: A or Space");
	ImGui::End();
}
#endif