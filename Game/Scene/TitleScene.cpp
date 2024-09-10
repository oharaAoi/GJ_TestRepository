#include "TitleScene.h"
#include "Engine/Game/Managers/SceneManager/SceneManager.h"

void TitleScene::initialize() {
	Input::GetInstance()->Init(WinApp::GetWNDCLASS(), WinApp::GetWndHandle());
	input_ = Input::GetInstance();

	camera3D_ = std::make_unique<Camera3D>();
	camera3D_->initialize();
	camera3D_->set_transform({
		CVector3::BASIS,
		Quaternion::EulerDegree(0, 0, 0),
		{ 0, 0, -10.0 }
							 });

	titleObject_ = std::make_unique<GameObject>();
	titleObject_->reset_object("Title.obj");
}

void TitleScene::load() {
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/TitleScene/Title", "Title.obj");
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

	camera3D_->update();
}

void TitleScene::begin_rendering() {
	camera3D_->begin_rendering(*camera3D_);
	camera3D_->update_matrix();

	titleObject_->begin_rendering(*camera3D_);
}

void TitleScene::late_update() {
}

void TitleScene::draw() const {
	RenderPathManager::BeginFrame();
	titleObject_->draw();
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

	ImGui::Begin("Camera3D");
	camera3D_->debug_gui();
	ImGui::End();

}
#endif
