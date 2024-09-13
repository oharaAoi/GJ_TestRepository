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

	AdjustmentItem::GetInstance()->Init("GameOver");

	Camera2D::Initialize();
	camera3D_ = std::make_unique<Camera3D>();
	camera3D_->initialize();
	camera3D_->set_transform({
			CVector3::BASIS,
			Quaternion::EulerDegree(0, 0, 0),
			{ 0, 0, -10.0 }
							 });

	gameOverUI_ = std::make_unique<GameOverUI>();

	nextGame_ = true;
	preNextGame_ = false;

	gameOverTitle_ = std::make_unique<GameObject>();
	gameOverTitle_->reset_object("gameover.obj");

	skydome_ = std::make_unique<GameObject>();
	skydome_->reset_object("skydome.obj");

	boss_ = std::make_unique<Boss>();
	boss_->get_transform().set_translate({-11.0f, -6.6f, 34.7f});
	boss_->get_transform().set_scale({0.1f, 0.1f, 0.1f });
	boss_->get_transform().set_rotate(Quaternion::EulerDegree(-90.0f, 0.0f, 0.0f));
	boss_->SetParentTransform("GameOverBoss");

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
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameOverScene/GameOver", "gameover.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/Skydome", "skydome.obj");

	TextureManager::RegisterLoadQue("./Game/Resources/UI", "Fade_Panel.png");
	TextureManager::RegisterLoadQue("./Game/Resources/UI", "gameoverRetryKey.png");
	TextureManager::RegisterLoadQue("./Game/Resources/UI", "gameoverTitleKey.png");
	TextureManager::RegisterLoadQue("./Game/Resources/UI", "gameoverCursolKey.png");

	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/BossFace", "bossFace.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/LowerJaw", "lowerJaw.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/UpperJaw", "upperJaw.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/InMouth", "InMouth.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/BossEyes", "bossEyes.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/BossEyesbrows", "bossEyebrows.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/OverLine", "OverLine.obj");

	AudioManager::RegisterLoadQue("./Game/Resources/Audio/gameOver", "meteOnigiri_start.wav");
	AudioManager::RegisterLoadQue("./Game/Resources/Audio/gameOver", "meteOnigiri_choice1.wav");
	AudioManager::RegisterLoadQue("./Game/Resources/Audio/gameOver", "meteOnigiri_AfterGameBGM2.wav");
}

void GameOverScene::begin() {
}

void GameOverScene::update() {
	camera3D_->update();
	Camera2D::CameraUpdate();

	AdjustmentItem::GetInstance()->Update();

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

	// -------------------------------------------------
	// ↓ Objectの更新
	// -------------------------------------------------
	gameOverTitle_->update();
	skydome_->update();

	bossMoveRotate_ = boss_->get_transform().get_quaternion();
	boss_->get_transform().set_rotate(bossMoveRotate_ * Quaternion::AngleAxis({ 0,0,1 }, (1.0f * ToRadian)));

	preNextGame_ = nextGame_;
}

void GameOverScene::begin_rendering() {
	camera3D_->begin_rendering(*camera3D_);
	camera3D_->update_matrix();

	gameOverTitle_->begin_rendering(*camera3D_);
	skydome_->begin_rendering(*camera3D_);

	boss_->Begin_Rendering(camera3D_.get());
	
	gameOverUI_->Begin_Rendering();

	fadePanel_->Begin_Rendering();
}

void GameOverScene::late_update() {
}

void GameOverScene::draw() const {
	RenderPathManager::BeginFrame();
	gameOverTitle_->draw();
	skydome_->draw();
	boss_->Draw();
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

	boss_->EditImGui();

	if (ImGui::Button("get")) {
		boss_->SetParentTransform("GameOverBoss");
	}
}
#endif