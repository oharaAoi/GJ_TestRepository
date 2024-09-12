#include "GameOverScene.h"
#include "Engine/Game/Managers/SceneManager/SceneManager.h"
#include "Engine/Game/Managers/TextureManager/TextureManager.h"
#include "Engine/Render/RenderTargetGroup/SwapChainRenderTargetGroup.h"


void GameOverScene::finalize() {
	object3DNode->finalize();
	outlineNode->finalize();
	spriteNode->finalize();
	RenderPathManager::UnregisterPath("GameOver");

	start_SE_->finalize();
	choice_SE_->finalize();
	gameOver_BGM_->finalize();
}

void GameOverScene::initialize() {
	input_ = Input::GetInstance();

	Camera2D::Initialize();
	camera3D_ = std::make_unique<Camera3D>();

	gameOverUI_ = std::make_unique<GameOverUI>();

	preNextGame_ = false;

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	object3DNode = std::make_unique<Object3DNode>();
	object3DNode->initialize();
	object3DNode->set_render_target();
	object3DNode->set_depth_stencil();

	outlineNode = std::make_unique<OutlineNode>();
	outlineNode->initialize();
	outlineNode->set_render_target();
	outlineNode->set_depth_resource(DirectXSwapChain::GetDepthStencil()->texture_gpu_handle());
	outlineNode->set_texture_resource(object3DNode->result_stv_handle());

	spriteNode = std::make_unique<SpriteNode>();
	spriteNode->initialize();
	spriteNode->set_background_texture(outlineNode->result_stv_handle());
	spriteNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());
	DirectXSwapChain::GetRenderTarget()->set_depth_stencil(nullptr);

	path.initialize({ object3DNode, outlineNode, spriteNode });
	RenderPathManager::RegisterPath("GameOver", std::move(path));
	RenderPathManager::SetPath("GameOver");

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	fadePanel_ = std::make_unique<FadePanel>();
	fadePanel_->SetFadeFadeStart(FadeType::Fade_Out);

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	start_SE_ = std::make_unique<AudioPlayer>();
	choice_SE_ = std::make_unique<AudioPlayer>();
	gameOver_BGM_ = std::make_unique<AudioPlayer>();

	start_SE_->initialize("meteOnigiri_start.wav" ,0.5f, false);
	choice_SE_->initialize("meteOnigiri_choice1.wav", 0.5f, false);
	gameOver_BGM_->initialize("meteOnigiri_AfterGameBGM2.wav", 0.5f, false);

	gameOver_BGM_->play();
}

void GameOverScene::load() {
	TextureManager::RegisterLoadQue("./Game/Resources/UI", "UI_goGame.png");
	TextureManager::RegisterLoadQue("./Game/Resources/UI", "UI_goTitle.png");
	TextureManager::RegisterLoadQue("./Game/Resources/UI", "UI_arrow.png");

	AudioManager::RegisterLoadQue("./Game/Resources/Audio/gameOver", "meteOnigiri_start.wav");
	AudioManager::RegisterLoadQue("./Game/Resources/Audio/gameOver", "meteOnigiri_choice1.wav");
	AudioManager::RegisterLoadQue("./Game/Resources/Audio/gameOver", "meteOnigiri_AfterGameBGM2.wav");
}

void GameOverScene::begin() {
}

void GameOverScene::update() {
	camera3D_->update();
	Camera2D::CameraUpdate();

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	fadePanel_->Update();
	gameOverUI_->Update(nextGame_);

	if (!fadePanel_->GetIsFadeFinish()) {
		return;
	}

	// -------------------------------------------------
	// ↓ Inputの更新
	// -------------------------------------------------
	Input::GetInstance()->Update();
	
	if (input_->GetIsPadTrigger(XINPUT_GAMEPAD_A)) {
		if (nextGame_) {
			start_SE_->play();
			gameOver_BGM_->stop();
			fadePanel_->SetFadeFadeStart(FadeType::Fade_In);
			SceneManager::SetSceneChange(CreateUnique<GameScene>(),
										 static_cast<float>((fadePanel_->GetFadeTime() + 10) * GameTimer::DeltaTime()),
										 false);
		} else {
			start_SE_->play();
			gameOver_BGM_->stop();
			fadePanel_->SetFadeFadeStart(FadeType::Fade_In);
			SceneManager::SetSceneChange(CreateUnique<TitleScene>(),
										 static_cast<float>((fadePanel_->GetFadeTime() + 10) * GameTimer::DeltaTime()),
										 false);
		}
	}

	Vector2 stick = input_->GetLeftJoyStick().normalize_safe();
	if (stick.y >= 0.5f) {
		nextGame_ = true;
	} else if(stick.y <= -0.5f) {
		nextGame_ = false;
	}

	if (preNextGame_ != nextGame_) {
		choice_SE_->restart();
	}

	preNextGame_ = nextGame_;
}

void GameOverScene::begin_rendering() {
	camera3D_->begin_rendering(*camera3D_);
	camera3D_->update_matrix();
	
	gameOverUI_->Begin_Rendering();

	fadePanel_->Begin_Rendering();
}

void GameOverScene::late_update() {
}

void GameOverScene::draw() const {
	RenderPathManager::BeginFrame();
	
	RenderPathManager::Next();
	outlineNode->draw();
	RenderPathManager::Next();
	gameOverUI_->Draw();
	fadePanel_->Draw();
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

	ImGui::Begin("Panel");
	fadePanel_->Debug_gui();
	ImGui::End();
}
#endif