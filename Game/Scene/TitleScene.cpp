#include "TitleScene.h"
#include "Engine/Game/Managers/SceneManager/SceneManager.h"
#include "Engine/Game/Managers/TextureManager/TextureManager.h"
#include "Engine/Render/RenderTargetGroup/SwapChainRenderTargetGroup.h"

void TitleScene::finalize() {
	object3DNode->finalize();
	outlineNode->finalize();
	spriteNode->finalize();
	RenderPathManager::UnregisterPath("GameScene");
}

void TitleScene::initialize() {
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
	RenderPathManager::RegisterPath("GameScene", std::move(path));
	RenderPathManager::SetPath("GameScene");
}

void TitleScene::load() {
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/TitleScene/Title", "Title.obj");

	TextureManager::RegisterLoadQue("./Game/Resources/UI", "Fade_Panel.png");
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
	Camera2D::CameraUpdate();

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	fadePanel_->Update();
}

void TitleScene::begin_rendering() {
	camera3D_->begin_rendering(*camera3D_);
	camera3D_->update_matrix();

	titleObject_->begin_rendering(*camera3D_);

	fadePanel_->Begin_Rendering();
}

void TitleScene::late_update() {
}

void TitleScene::draw() const {
	RenderPathManager::BeginFrame();
	titleObject_->draw();
	RenderPathManager::Next();
	outlineNode->draw();
	RenderPathManager::Next();
	fadePanel_->Draw();
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
