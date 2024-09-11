#include "TutorialScene.h"
#include "Engine/Game/Managers/SceneManager/SceneManager.h"
#include "Engine/Game/Managers/TextureManager/TextureManager.h"

void TutorialScene::initialize() {
	input_ = Input::GetInstance();

	AdjustmentItem::GetInstance()->Init("TutorialScene");

	Camera2D::Initialize();
	camera3D_ = std::make_unique<Camera3D>();
	camera3D_->initialize();
	camera3D_->set_transform({
		CVector3::BASIS,
		Quaternion::EulerDegree(55, 0, 0),
		{ 0, 50, -28.0 }
	});

	field_ = std::make_unique<Field>();
	player_ = std::make_unique<Player>();
	player_->set_parent(field_->get_hierarchy());
	boss_ = std::make_unique<Boss>();

	//meteoriteManager_ = std::make_unique<MeteoriteManager>();
	//meteoriteManager_->SetGameScene(this);
	//meteoriteManager_->Init();


}

void TutorialScene::finalize() {
}

void TutorialScene::load() {
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources", "player.obj");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources", "particle.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/Models", "GravityRod.obj");

	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/BossFace", "bossFace.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/LowerJaw", "lowerJaw.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/UpperJaw", "upperJaw.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/InMouth", "InMouth.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/BossEyes", "bossEyes.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/BossEyesbrows", "bossEyebrows.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/OverLine", "OverLine.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/TriangleRiceBall", "triangleRiceBall.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/CircleRiceBall", "circleRiceBall.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/Field", "nattomaki.obj");

	TextureManager::RegisterLoadQue("./Game/Resources/UI", "UI_PlayerControl_move.png");
	TextureManager::RegisterLoadQue("./Game/Resources/UI", "UI_PlayerControl_attack.png");

	AudioManager::RegisterLoadQue("./Game/Resources/Audio", "SE_enemyEachOther.wav");
	AudioManager::RegisterLoadQue("./Game/Resources/Audio", "SE_meteoEachOther.wav");
	AudioManager::RegisterLoadQue("./Game/Resources/Audio", "SE_bossHited.wav");
	AudioManager::RegisterLoadQue("./Game/Resources/Audio", "SE_enemyAttack.wav");
	AudioManager::RegisterLoadQue("./Game/Resources/Audio", "SE_enemyHitToMeteo.wav");
	AudioManager::RegisterLoadQue("./Game/Resources/Audio", "SE_fieldPush.wav");
}

void TutorialScene::begin() {
}

void TutorialScene::update() {
	// -------------------------------------------------
	// ↓ Inputの更新
	// -------------------------------------------------
	Input::GetInstance()->Update();

	// -------------------------------------------------
	// ↓ 次のシーンに行くかの判定
	// -------------------------------------------------
	if (input_->GetIsPadTrigger(XINPUT_GAMEPAD_A) || input_->GetKey(DIK_SPACE)) {
		SceneManager::SetSceneChange(CreateUnique<GameScene>(), false);
	}

	// -------------------------------------------------
	// ↓ カメラを更新
	// -------------------------------------------------
	camera3D_->update();
	Camera2D::CameraUpdate();

	// -------------------------------------------------
	// ↓ GameObjectの更新
	// -------------------------------------------------
	field_->Update();
	player_->Update(field_->GetRadius());
	boss_->Update();

	// -------------------------------------------------
	// ↓ Manager系の更新
	// -------------------------------------------------


	// -------------------------------------------------
	// ↓ 当たり判定系
	// -------------------------------------------------
}

void TutorialScene::begin_rendering() {
	camera3D_->begin_rendering(*camera3D_);
	camera3D_->update_matrix();

	field_->begin_rendering(*camera3D_);
	player_->Begin_Rendering(camera3D_.get());
	boss_->Begin_Rendering(camera3D_.get());
}

void TutorialScene::late_update() {
}

void TutorialScene::draw() const {
	RenderPathManager::BeginFrame();
	field_->draw();
	player_->Draw();
	boss_->Draw();
#ifdef _DEBUG
	
#endif
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

	ImGui::Begin("Camera3D");
	camera3D_->debug_gui();
	ImGui::End();

	ImGui::Begin("Field");
	field_->EditImGui();
	ImGui::End();

	boss_->EditImGui();


}
#endif // _DEBUG