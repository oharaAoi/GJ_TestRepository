#include "GameOverScene.h"
#include "Engine/Game/Managers/SceneManager/SceneManager.h"

void GameOverScene::initialize() {
	input_ = Input::GetInstance();

	camera3D_ = std::make_unique<Camera3D>();
}

void GameOverScene::load() {
}

void GameOverScene::begin() {
}

void GameOverScene::update() {
	// -------------------------------------------------
	// ↓ Inputの更新
	// -------------------------------------------------
	Input::GetInstance()->Update();


	if (input_->GetIsPadTrigger(XINPUT_GAMEPAD_A) || input_->GetKey(DIK_SPACE)) {
		SceneManager::SetSceneChange(CreateUnique<TutorialScene>(), false);
	}
}

void GameOverScene::begin_rendering() {
	camera3D_->update_matrix();
}

void GameOverScene::late_update() {
}

void GameOverScene::draw() const {
	RenderPathManager::BeginFrame();

	RenderPathManager::Next();
}

#ifdef _DEBUG
#include <externals/imgui/imgui.h>
void GameOverScene::debug_update() {
	ImGui::Begin("GameOver");
	ImGui::Text("nowScene: GameOver");
	ImGui::Text("push: A or Space");
	ImGui::End();
}
#endif