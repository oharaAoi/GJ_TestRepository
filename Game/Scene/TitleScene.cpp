#include "TitleScene.h"
#include "Engine/Game/Managers/SceneManager/SceneManager.h"

void TitleScene::initialize() {
	Input::GetInstance()->Init(WinApp::GetWNDCLASS(), WinApp::GetWndHandle());
	input_ = Input::GetInstance();

	camera3D_ = std::make_unique<Camera3D>();
}

void TitleScene::load() {
}

void TitleScene::begin() {
}

void TitleScene::update() {
	// -------------------------------------------------
	// ↓ Inputの更新
	// -------------------------------------------------
	Input::GetInstance()->Update();


	if (input_->GetIsPadTrigger(XINPUT_GAMEPAD_A) || input_->GetKey(DIK_SPACE)) {
		SceneManager::SetSceneChange(CreateUnique<TutorialScene>(), false);
	}
}

void TitleScene::begin_rendering() {
	camera3D_->update_matrix();
}

void TitleScene::late_update() {
}

void TitleScene::draw() const {
	RenderPathManager::BeginFrame();

	RenderPathManager::Next();
}

#ifdef _DEBUG
#include <externals/imgui/imgui.h>
void TitleScene::debug_update() {
	ImGui::Begin("Title");
	ImGui::Text("nowScene: Title");
	ImGui::Text("nextScene: Tutorial");
	ImGui::Text("push: A or Space");
	ImGui::End();
}
#endif
