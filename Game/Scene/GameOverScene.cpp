#include "GameOverScene.h"
#include "Engine/Game/Managers/SceneManager/SceneManager.h"
#include "Engine/Game/Managers/TextureManager/TextureManager.h"



void GameOverScene::initialize() {
	input_ = Input::GetInstance();

	Camera2D::Initialize();
	camera3D_ = std::make_unique<Camera3D>();

	gameOverUI_ = std::make_unique<GameOverUI>();

	object3DNode = std::make_unique<Object3DNode>();
	object3DNode->initialize();
	object3DNode->set_render_target();

	spriteNode = std::make_unique<SpriteNode>();
	spriteNode->initialize();
	spriteNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());

	path.initialize({ object3DNode,spriteNode });
	RenderPathManager::RegisterPath("overSceneTest", std::move(path));
	RenderPathManager::SetPath("overSceneTest");
}

void GameOverScene::load() {
	TextureManager::RegisterLoadQue("./Game/Resources/UI", "UI_goGame.png");
	TextureManager::RegisterLoadQue("./Game/Resources/UI", "UI_goTitle.png");
	TextureManager::RegisterLoadQue("./Game/Resources/UI", "UI_arrow.png");
}

void GameOverScene::begin() {
}

void GameOverScene::update() {
	// -------------------------------------------------
	// ↓ Inputの更新
	// -------------------------------------------------
	Input::GetInstance()->Update();
	Camera2D::CameraUpdate();

	if (input_->GetIsPadTrigger(XINPUT_GAMEPAD_A) || input_->GetKey(DIK_SPACE)) {
		if (nextGame_) {
			SceneManager::SetSceneChange(CreateUnique<GameScene>(), false);
		} else {
			SceneManager::SetSceneChange(CreateUnique<TitleScene>(), false);
		}
	}

	Vector2 stick = input_->GetLeftJoyStick().normalize_safe();
	if (stick.y >= 0.5f) {
		nextGame_ = true;
	} else if(stick.y <= -0.5f) {
		nextGame_ = false;
	}

	gameOverUI_->Update(nextGame_);
}

void GameOverScene::begin_rendering() {
	camera3D_->update_matrix();

	gameOverUI_->Begin_Rendering();
}

void GameOverScene::late_update() {
}

void GameOverScene::draw() const {
	RenderPathManager::BeginFrame();

	RenderPathManager::Next();
	gameOverUI_->Draw();
	RenderPathManager::Next();
}

#ifdef _DEBUG
#include <externals/imgui/imgui.h>
void GameOverScene::debug_update() {
	ImGui::Begin("GameOver");
	ImGui::Text("nowScene: GameOver");
	ImGui::Text("push: A or Space");
	
	gameOverUI_->Debug_gui();

	ImGui::End();
}
#endif