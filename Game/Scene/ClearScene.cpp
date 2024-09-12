#include "ClearScene.h"
#include "Engine/Game/Managers/SceneManager/SceneManager.h"
#include "Engine/Render/RenderTargetGroup/SwapChainRenderTargetGroup.h"
#include "Engine/Game/Managers/TextureManager/TextureManager.h"

void ClearScene::finalize() {
	object3DNode->finalize();
	outlineNode->finalize();
	spriteNode->finalize();
	RenderPathManager::UnregisterPath("ClearScene");
}

void ClearScene::initialize() {
	input_ = Input::GetInstance();
	camera3D_ = std::make_unique<Camera3D>();

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	fadePanel_ = std::make_unique<FadePanel>();
	fadePanel_->SetFadeFadeStart(FadeType::Fade_Out);

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
}

void ClearScene::load() {
	TextureManager::RegisterLoadQue("./Game/Resources/UI", "Fade_Panel.png");
}

void ClearScene::begin() {
}

void ClearScene::update() {
	camera3D_->update();
	Camera2D::CameraUpdate();

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	fadePanel_->Update();

	if (!fadePanel_->GetIsFadeFinish()) {
		return;
	}
	// -------------------------------------------------
	// ↓ Inputの更新
	// -------------------------------------------------
	Input::GetInstance()->Update();

	if (input_->GetIsPadTrigger(XINPUT_GAMEPAD_A) || input_->GetKey(DIK_SPACE)) {
		fadePanel_->SetFadeFadeStart(FadeType::Fade_In);
		SceneManager::SetSceneChange(CreateUnique<TitleScene>(), 
									 static_cast<float>(fadePanel_->GetFadeTime() * GameTimer::DeltaTime()),
									 false);
	}
}

void ClearScene::begin_rendering() {
	camera3D_->begin_rendering(*camera3D_);
	camera3D_->update_matrix();

	fadePanel_->Begin_Rendering();
}

void ClearScene::late_update() {
}

void ClearScene::draw() const {
	RenderPathManager::BeginFrame();

	RenderPathManager::Next();
	outlineNode->draw();
	RenderPathManager::Next();
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
}
#endif