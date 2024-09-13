#include "ClearScene.h"
#include "Engine/Game/Managers/SceneManager/SceneManager.h"
#include "Engine/Render/RenderTargetGroup/SwapChainRenderTargetGroup.h"
#include "Engine/Game/Managers/TextureManager/TextureManager.h"
#include "Engine/Game/Managers/AudioManager/AudioManager.h"

void ClearScene::finalize() {
	object3DNode->finalize();
	outlineNode->finalize();
	spriteNode->finalize();
	RenderPathManager::UnregisterPath("ClearScene");
}

void ClearScene::initialize() {
	input_ = Input::GetInstance();
	Camera2D::Initialize();
	camera3D_ = std::make_unique<Camera3D>();
	camera3D_->initialize();
	camera3D_->set_transform({
			CVector3::BASIS,
			Quaternion::EulerDegree(0, 0, 0),
			{ 0, 0, -10.0 }
							 });

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	fadePanel_ = std::make_unique<FadePanel>();
	fadePanel_->SetFadeFadeStart(FadeType::Fade_Out);

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	clearUI_ = std::make_unique<ClearUI>();

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	clearTitle_ = std::make_unique<GameObject>();
	clearTitle_->reset_object("clear.obj");

	skydome_ = std::make_unique<GameObject>();
	skydome_->reset_object("skydome.obj");

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
	RenderPathManager::RegisterPath("ClearScene", std::move(path));
	RenderPathManager::SetPath("ClearScene");

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	start_SE_ = std::make_unique<AudioPlayer>();
	clear_BGM_ = std::make_unique<AudioPlayer>();
	choice_SE_ = std::make_unique<AudioPlayer>();
	start_SE_->initialize("meteOnigiri_start.wav", 0.5f, false);
	clear_BGM_->initialize("meteOnigiri_AfterGameBGM4.wav", 0.5f, true);
	choice_SE_->initialize("meteOnigiri_choice1.wav", 0.5f, false);

	clear_BGM_->play();
}

void ClearScene::load() {
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/ClearScene/Clear", "clear.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/Skydome", "skydome.obj");

	TextureManager::RegisterLoadQue("./Game/Resources/UI", "Fade_Panel.png");
	TextureManager::RegisterLoadQue("./Game/Resources/UI", "gameoverRetryKey.png");
	TextureManager::RegisterLoadQue("./Game/Resources/UI", "gameoverTitleKey.png");
	TextureManager::RegisterLoadQue("./Game/Resources/UI", "gameoverCursolKey.png");

	AudioManager::RegisterLoadQue("./Game/Resources/Audio/clear", "meteOnigiri_start.wav");
	AudioManager::RegisterLoadQue("./Game/Resources/Audio/clear", "meteOnigiri_AfterGameBGM4.wav");
	AudioManager::RegisterLoadQue("./Game/Resources/Audio/clear", "meteOnigiri_choice1.wav");
}

void ClearScene::begin() {
}

void ClearScene::update() {
	camera3D_->update();
	Camera2D::CameraUpdate();

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	clearUI_->Update(nextGame_);
	fadePanel_->Update();

	if (!fadePanel_->GetIsFadeFinish()) {
		return;
	}
	// -------------------------------------------------
	// ↓ Inputの更新
	// -------------------------------------------------
	Input::GetInstance()->Update();

	if (input_->GetIsPadTrigger(XINPUT_GAMEPAD_A)) {
		if (nextGame_) {	// gameへ
			start_SE_->play();
			clear_BGM_->stop();
			fadePanel_->SetFadeFadeStart(FadeType::Fade_In);
			SceneManager::SetSceneChange(CreateUnique<GameScene>(),
										 static_cast<float>((fadePanel_->GetFadeTime() + 10) * GameTimer::DeltaTime()),
										 false);
		} else {	// Titleへ
			start_SE_->play();
			clear_BGM_->stop();
			fadePanel_->SetFadeFadeStart(FadeType::Fade_In);
			SceneManager::SetSceneChange(CreateUnique<TitleScene>(),
										 static_cast<float>((fadePanel_->GetFadeTime() + 10) * GameTimer::DeltaTime()),
										 false);
		}
	}

	// -------------------------------------------------
	// ↓ 矢印の移動
	// -------------------------------------------------
	Vector2 stick = input_->GetLeftJoyStick().normalize_safe();
	if (stick.y >= 0.5f) {
		nextGame_ = true;
	} else if (stick.y <= -0.5f) {
		nextGame_ = false;
	}

	if (preNextGame_ != nextGame_) {
		choice_SE_->restart();
	}

	// -------------------------------------------------
	// ↓ worldObjectの更新
	// -------------------------------------------------

	clearTitle_->update();
	skydome_->update();


	preNextGame_ = nextGame_;
}

void ClearScene::begin_rendering() {
	camera3D_->begin_rendering(*camera3D_);
	camera3D_->update_matrix();

	clearTitle_->begin_rendering(*camera3D_);
	skydome_->begin_rendering(*camera3D_);

	clearUI_->Begin_Rendering();

	fadePanel_->Begin_Rendering();
}

void ClearScene::late_update() {
}

void ClearScene::draw() const {
	RenderPathManager::BeginFrame();
	clearTitle_->draw();
	skydome_->draw();
	RenderPathManager::Next();
	outlineNode->draw();
	RenderPathManager::Next();
	clearUI_->Draw();
	fadePanel_->Draw();
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

	ImGui::Begin("Panel");
	fadePanel_->Debug_gui();
	ImGui::End();

	ImGui::Begin("UI");
	clearUI_->Debug_gui();
	ImGui::End();

	ImGui::Begin("Object");
	clearTitle_->debug_gui();
	ImGui::End();
}
#endif