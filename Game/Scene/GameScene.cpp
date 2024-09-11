#include "GameScene.h"
#include "Game/Enviroment.h"
#include "Engine/Game/Managers/SceneManager/SceneManager.h"
#include "Engine/Utility/Utility.h"
#include "Engine/Render/RenderPathManager/RenderPathManager.h"
#include "Engine/DirectX/DirectXCore.h"
#include "Engine/Game/Managers/TextureManager/TextureManager.h"
#include "Engine/Render/RenderTargetGroup/SwapChainRenderTargetGroup.h"

#ifdef _DEBUG
#include "Game/Editor/EditorController.h"
#endif // _DEBUG

GameScene::GameScene() = default;

GameScene::~GameScene() = default;

void GameScene::initialize() {
	Input::GetInstance()->Init(WinApp::GetWNDCLASS(), WinApp::GetWndHandle());
	EffectManager::GetInstance()->Init();
	AdjustmentItem::GetInstance()->Init("GameScene");

	effectManager_ = EffectManager::GetInstance();
	collisionManager_ = std::make_unique<CollisionManager>();

	field_ = std::make_unique<Field>();
	player_ = std::make_unique<Player>();
	player_->set_parent(field_->get_hierarchy());
	boss_ = std::make_unique<Boss>();

	Camera2D::Initialize();
	camera3D_ = std::make_unique<FollowCamera>();
	camera3D_->initialize();
	camera3D_->set_transform({
		CVector3::BASIS,
		Quaternion::EulerDegree(55, 0, 0),
		{ 0, 50, -28.0 }
		});

	collisionManager_->register_collider("Player", player_->GetCollider());
	meteoriteManager_ = std::make_unique<MeteoriteManager>(meteoriteList_, collisionManager_.get());
	enemyManager_ = std::make_unique<EnemyManager>(enemyList_, collisionManager_.get(), player_->GetIsAttackofEnmey());

	playerUI_ = std::make_unique<PlayerUI>();

	object3DNode = std::make_unique<Object3DNode>();
	object3DNode->initialize();
	object3DNode->set_render_target();
	object3DNode->set_depth_stencil();

	spriteNode = std::make_unique<SpriteNode>();
	spriteNode->initialize();
	spriteNode->set_background_texture(object3DNode->result_stv_handle());
	spriteNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());
	DirectXSwapChain::GetRenderTarget()->set_depth_stencil(nullptr);

	path.initialize({ object3DNode,spriteNode });
	RenderPathManager::RegisterPath("GameScene", std::move(path));
	RenderPathManager::SetPath("GameScene");

#ifdef _DEBUG
	editor = CreateUnique<EditorController>();
	editor->initialize(camera3D_.get(), meteoriteManager_.get(), enemyManager_.get());
	meteoriteManager_->SetEditor(editor.get());
#endif // _DEBUG

}

void GameScene::load() {
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources", "Planet.obj");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources", "player.obj");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources", "particle.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/Models", "GravityRod.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/Models", "mouth.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/Models", "mob.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/Models", "Field.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/Models", "kariEnemy.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/Models", "kariSpEnemy.obj");

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

void GameScene::begin() {
}

void GameScene::update() {

	// -------------------------------------------------
	// ↓ Inputの更新
	// -------------------------------------------------
	Input::GetInstance()->Update();

	AdjustmentItem::GetInstance()->Update();

	// -------------------------------------------------
	// ↓ カメラを更新
	// -------------------------------------------------
	camera3D_->update();
	Camera2D::CameraUpdate();

	// -------------------------------------------------
	// ↓ ゲームクリア/オーバー確認
	// -------------------------------------------------
	if (boss_->GetIsClear()) {
		SceneManager::SetSceneChange(CreateUnique<ClearScene>(), false);
	}

	if (boss_->GetIsGameOver(field_->GetCylinderHight())) {
		SceneManager::SetSceneChange(CreateUnique<GameOverScene>(), false);
	}

	// -------------------------------------------------
	// ↓ GameObjectの更新
	// -------------------------------------------------
	field_->Update();

	player_->Update(field_->GetRadius());

#ifdef _DEBUG
	if (!editor->is_edit()) {
		boss_->Update();
	}
#else
	boss_->Update();
#endif // _DEBUG

	for (std::unique_ptr<Meteorite>& meteo : meteoriteList_) {
		meteo->Update(player_->get_transform().get_translate());
	}

	// 死亡フラグのチェックを行う
	meteoriteList_.remove_if([](const std::unique_ptr<Meteorite>& meteo) {
		if (meteo->GetIsDead()) {
			return true;
		}
		return false;
	});

	for (std::unique_ptr<Enemy>& enemy : enemyList_) {
		enemy->Update(player_->get_transform().get_translate());
	}

	enemyList_.remove_if([](const std::unique_ptr<Enemy>& enemy) {
		if (enemy->GetIsDead()) {
			return true;
		}
		return false;
	});

	// -------------------------------------------------
	// ↓ Manager系の更新
	// -------------------------------------------------
	effectManager_->Update();

	meteoriteManager_->Update(player_->get_transform().get_translate());

	enemyManager_->Update(player_->get_transform().get_translate());

	// -------------------------------------------------
	// ↓ 当たり判定系
	// -------------------------------------------------
	if (player_->GetIsAttack()) {
		CheckMeteoAttraction();
	}
	CheckBossCollision();

	CheckMeteoToField();


}

void GameScene::begin_rendering() {

	camera3D_->begin_rendering(*camera3D_);
	camera3D_->update_matrix();

	playerUI_->Update(player_->world_position(), camera3D_->vp_matrix(), player_->GetIsAttack());

	field_->begin_rendering(*camera3D_);
	player_->Begin_Rendering(camera3D_.get());
	boss_->Begin_Rendering(camera3D_.get());

	for (std::unique_ptr<Meteorite>& meteo : meteoriteList_) {
		meteo->begin_rendering(*camera3D_);
	}

	for (std::unique_ptr<Enemy>& enemy : enemyList_) {
		enemy->begin_rendering(*camera3D_);
	}

	effectManager_->BeginRendering(*camera3D_);

#ifdef _DEBUG
	enemyManager_->Begin_Rendering(*camera3D_);
#endif

	playerUI_->BeginRendering();
}

void GameScene::late_update() {
	collisionManager_->update();

	// 敵とPlayer --------------------------------------------------
	collisionManager_->collision("Enemy", "Player");
	// メテオ同士 ---------------------------------------------------
	collisionManager_->collision("Meteo", "Meteo");
	// Enemy同士 ---------------------------------------------------
	collisionManager_->collision("Enemy", "Enemy");
	// Enemyと隕石 -------------------------------------------------
	collisionManager_->collision("Enemy", "Meteo");
}

void GameScene::draw() const {
	RenderPathManager::BeginFrame();
	//DirectXCore::ShowGrid(*camera3D_);
	field_->draw();
	player_->Draw();
	boss_->Draw();

	for (const std::unique_ptr<Meteorite>& meteo : meteoriteList_) {
		meteo->draw();
	}

	for (const std::unique_ptr<Enemy>& enemy : enemyList_) {
		enemy->draw();
	}

	effectManager_->Draw();

#ifdef _DEBUG
	enemyManager_->Draw();
	collisionManager_->debug_draw3d(*camera3D_);
	editor->draw_debug3d();
#endif
	RenderPathManager::Next();
	playerUI_->Draw();
	RenderPathManager::Next();
}

void GameScene::CheckMeteoToField() {
	for (std::unique_ptr<Meteorite>& meteo : meteoriteList_) {
		if (meteo->GetIsFalling()) {
			// 円柱に面の上にあるか
			Vector3 meteoPos = meteo->get_transform().get_translate();
			meteoPos.y = 0;
			float length = Vector3::Length(meteoPos - Vector3{ 0, 0,0 });

			// 円の範囲内に隕石がある
			if (length > field_->GetRadius() + meteo->GetRadius()) {
				continue;
			}

			// 隕石が面と同じ高さにある(面に当たったら円柱を貫通では押し込まれない対策)
			if (meteo->get_transform().get_translate().y < 12.5f && meteo->get_transform().get_translate().y > 10.0f) {
				meteo->SetIsDead(true);
				field_->SetVelocityY(-3.0f);
				boss_->OnCollision();
				boss_->PlayFieldPushSE();
			}
		}
	}
}

void GameScene::CheckMeteoAttraction() {
	for (std::unique_ptr<Meteorite>& meteo : meteoriteList_) {
		Vector3 direction{};
		float length = 0;
		// 引き寄せる2つの球体との距離を測る
		Vector3 meteoToAttractOrigine = player_->GetGravityRodOrigine() - meteo->world_position();
		Vector3 meteoToAttractEnd = player_->GetGravityRodEnd() - meteo->world_position();

		float origineLength = meteoToAttractOrigine.length();
		float endLength = meteoToAttractEnd.length();

		if (origineLength < meteo->GetAttractRange() || endLength < meteo->GetAttractRange()) {

			meteo->SetIsAttraction(true);
			// 距離を比較して近い方とのベクトルを加速度に加算する
			if (origineLength < endLength) {
				meteo->SetAcceleration(Vector3::Normalize(meteoToAttractOrigine));
				length = origineLength;
				direction = Vector3::Normalize(meteoToAttractOrigine);
				meteo->SetTargetPosition(player_->GetGravityRodOrigine());
			}
			else {
				meteo->SetAcceleration(Vector3::Normalize(meteoToAttractEnd));
				length = endLength;
				direction = Vector3::Normalize(meteoToAttractEnd);
				meteo->SetTargetPosition(player_->GetGravityRodEnd());
			}
		}
		else {
			meteo->SetIsAttraction(false);
		}
	}
}

void GameScene::CheckBossCollision() {
	for (std::unique_ptr<Meteorite>& meteo : meteoriteList_) {
		if (meteo->GetIsFalling()) {
			float length = (meteo->get_transform().get_translate().y - boss_->get_transform().get_translate().y);

			if (length < meteo->GetRadius()) {
				meteo->SetIsDead(true);
				boss_->OnCollision();
			}
		}
	}
}

#ifdef _DEBUG

#include <externals/imgui/imgui.h>

void GameScene::debug_update() {
	ImGui::Begin("Camera3D");
	camera3D_->debug_gui();
	ImGui::End();

	ImGui::Begin("field");
	if (ImGui::Button("exit")) {
		field_->SetVelocityY(-3.0f);
		boss_->OnCollision();
	}
	ImGui::End();

	field_->EditImGui();

	ImGui::Begin("GameObject");
	player_->debug_gui();
	ImGui::End();

	boss_->EditImGui();

	ImGui::Begin("Meteorite");
	ImGui::DragFloat("attractionedStrength", &Meteorite::kAttractionedStrength_, 0.1f, 0.0f, 200.0f);
	ImGui::DragFloat("kSpeed", &Meteorite::kSpeed_, 0.1f, 0.0f, 5.0f);
	ImGui::End();

	editor->draw_gui();

	meteoriteManager_->DebugGui();
}
#endif // _DEBUG
