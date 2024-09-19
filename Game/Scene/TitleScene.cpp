#include "TitleScene.h"
#include "Engine/Game/Managers/SceneManager/SceneManager.h"
#include "Engine/Game/Managers/TextureManager/TextureManager.h"
#include "Engine/Render/RenderTargetGroup/SwapChainRenderTargetGroup.h"
#include "Engine/Game/Managers/AudioManager/AudioManager.h"

#include "Engine/DirectX/DirectXCore.h"

void TitleScene::finalize() {
	object3DNode->finalize();
	outlineNode->finalize();
	spriteNode->finalize();
	RenderPathManager::UnregisterPath("GameScene");

	title_BGM_->finalize();
	start_SE_->finalize();
}

void TitleScene::initialize() {
	DirectXCore::Get3DLight().direction = CVector3::BASIS_Z;
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
	titleObject_ = std::make_unique<GameObject>();
	titleObject_->reset_object("Title.obj");

	skydome_ = std::make_unique<GameObject>();
	skydome_->reset_object("skydome.obj");

	player_ = std::make_unique<Player>();
	player_->get_transform().set_translate({ 2.6f, -2.0f, -2.0f});
	player_->get_transform().set_rotate(Quaternion::EulerDegree({ 0.0f, -140.0f, 0.0f }));

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	fadePanel_ = std::make_unique<FadePanel>();
	fadePanel_->SetFadeFadeStart(FadeType::Fade_Out);

	startUI_ = std::make_unique<UIObject>("startKey.png", Vector2{ 0.5f, 0.5f });

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
	RenderPathManager::RegisterPath("GameScene", std::move(path));
	RenderPathManager::SetPath("GameScene");

	start_SE_ = std::make_unique<AudioPlayer>();
	start_SE_->initialize("meteOnigiri_start.wav", 0.5f, false);
	title_BGM_ = std::make_unique<AudioPlayer>();
	title_BGM_->initialize("meteOnigiri_titleBGM.wav", 0.5f, true);
	title_BGM_->play();

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	isFall_ = true;
	frameCount_ = 0;
	frameTime_ = 120.0f;

	titleObject_->get_transform().set_translate_y(20);
}

void TitleScene::load() {
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/TitleScene/Title", "Title.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/Skydome", "skydome.obj");

	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/PlayerBody", "playerBody.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/PlayerArm", "playerArm.obj");

	TextureManager::RegisterLoadQue("./Game/Resources/UI", "Fade_Panel.png");
	TextureManager::RegisterLoadQue("./Game/Resources/UI", "startKey.png");

	AudioManager::RegisterLoadQue("./Game/Resources/Audio/title", "meteOnigiri_start.wav");
	AudioManager::RegisterLoadQue("./Game/Resources/Audio/title", "meteOnigiri_titleBGM.wav");
}

void TitleScene::begin() {
}

void TitleScene::update() {
	camera3D_->update();
	Camera2D::CameraUpdate();

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	fadePanel_->Update();
	startUI_->Update({ 0.4f, 0.4f }, { 640.0f, 520.0f }, 1.0f);
	player_->Floating();

	if (!fadePanel_->GetIsFadeFinish()) {
		return;
	}

	if (isFall_) {
		frameTime_ = 120.0f;
		FallTitle();
		return;
	}

	skydome_->update();

	// -------------------------------------------------
	// ↓ Inputの更新
	// -------------------------------------------------
	Input::GetInstance()->Update();

	if (!fadePanel_->GetIsFade()) {
		if (input_->GetIsPadTrigger(XINPUT_GAMEPAD_A)) {
			start_SE_->play();
			title_BGM_->stop();
			fadePanel_->SetFadeFadeStart(FadeType::Fade_In);
			SceneManager::SetSceneChange(CreateUnique<TutorialScene>(),
				fadePanel_->GetFadeTime() + 0.1f,
				false);
		}
	}
}

void TitleScene::begin_rendering() {
	camera3D_->begin_rendering(*camera3D_);
	camera3D_->update_matrix();

	skydome_->begin_rendering(*camera3D_);
	titleObject_->begin_rendering(*camera3D_);
	player_->Begin_Rendering(camera3D_.get());

	startUI_->begin_rendering();
	fadePanel_->Begin_Rendering();
}

void TitleScene::late_update() {
}

void TitleScene::draw() const {
	RenderPathManager::BeginFrame();
	skydome_->draw();
	titleObject_->draw();
	player_->Draw();
	RenderPathManager::Next();
	outlineNode->draw();
	RenderPathManager::Next();
	startUI_->draw();
	fadePanel_->Draw();
	RenderPathManager::Next();
}

void TitleScene::FallTitle() {
	if (t <= 1.0f) {
		t += 0.7f * GameTimer::DeltaTime();
		float height = std::lerp(6.0f, 0.0f, EaseInOut::Bounce(t));
		titleObject_->get_transform().set_translate_y(height);
	}
	else {
		isFall_ = false;
	}
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

	ImGui::Begin("Panel");
	fadePanel_->Debug_gui();
	ImGui::End();

	player_->Debug_Gui();
}
#endif
