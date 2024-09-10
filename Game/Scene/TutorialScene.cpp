#include "TutorialScene.h"
#include "Engine/Game/Managers/SceneManager/SceneManager.h"

void TutorialScene::initialize() {
	input_ = Input::GetInstance();

	camera3D_ = std::make_unique<Camera3D>();
}

void TutorialScene::load() {
}

void TutorialScene::begin() {
}

void TutorialScene::update() {
	// -------------------------------------------------
	// ↓ Inputの更新
	// -------------------------------------------------
	Input::GetInstance()->Update();


	if (input_->GetIsPadTrigger(XINPUT_GAMEPAD_A) || input_->GetKey(DIK_SPACE)) {
		SceneManager::SetSceneChange(CreateUnique<GameScene>(), false);
	}

	camera3D_->update();
}

void TutorialScene::begin_rendering() {
	camera3D_->begin_rendering(*camera3D_);
	camera3D_->update_matrix();
}

void TutorialScene::late_update() {
}

void TutorialScene::draw() const {
	RenderPathManager::BeginFrame();

	RenderPathManager::Next();
}


#ifdef _DEBUG
#include <externals/imgui/imgui.h>
void TutorialScene::debug_update() {
	ImGui::Begin("Tutorial");
	ImGui::Text("nowScene: Tutorial");
	ImGui::Text("nextScene: Game");
	ImGui::Text("push: A or Space");
	ImGui::End();
}
#endif // _DEBUG