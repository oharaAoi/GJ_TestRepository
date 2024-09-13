#include "GameScene.h"
#include "Game/Enviroment.h"
#include "Engine/Game/Managers/SceneManager/SceneManager.h"
#include "Engine/Utility/Utility.h"
#include "Engine/Render/RenderPathManager/RenderPathManager.h"
#include "Engine/DirectX/DirectXCore.h"
#include "Engine/Game/Managers/TextureManager/TextureManager.h"
#include "Engine/Render/RenderTargetGroup/SwapChainRenderTargetGroup.h"
#include "Engine/Game/Managers/AudioManager/AudioManager.h"

#ifdef _DEBUG
#include "Game/Editor/EditorController.h"
#endif // _DEBUG

GameScene::GameScene() = default;

GameScene::~GameScene() = default;

void GameScene::finalize() {
	object3DNode->finalize();
	outlineNode->finalize();
	spriteNode->finalize();
	vignetteNode->finalize();
	RenderPathManager::UnregisterPath("GameScene");

	// followカメラにある音声を解放
	camera3D_->finalize();
	game_BGM_->finalize();
}

void GameScene::initialize() {
	EffectManager::GetInstance()->Init();
	AdjustmentItem::GetInstance()->Init("GameScene");

	effectManager_ = EffectManager::GetInstance();
	collisionManager_ = std::make_unique<CollisionManager>();

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	Camera2D::Initialize();
	camera3D_ = std::make_unique<FollowCamera>();
	camera3D_->initialize();
	camera3D_->set_transform({
		CVector3::BASIS,
		Quaternion::EulerDegree(55, 0, 0),
		{ 0, 30, -13.0 }
							 });
	camera3D_->begin_rendering(*camera3D_);
	camera3D_->update_matrix();

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	performanceType_ = PerformanceType::GameStart_Type;

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	skydome_ = std::make_unique<GameObject>();
	skydome_->reset_object("skydome.obj");

	field_ = std::make_unique<Field>();

	satisFaction_ = std::make_unique<SatisFactionLevel>();

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	player_ = std::make_unique<Player>();
	player_->set_parent(field_->get_hierarchy());
	boss_ = std::make_unique<Boss>();

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	collisionManager_->register_collider("Player", player_->GetCollider());
	meteoriteManager_ = std::make_unique<MeteoriteManager>(meteoriteList_, collisionManager_.get());
	enemyManager_ = std::make_unique<EnemyManager>(enemyList_, collisionManager_.get(), player_->GetIsAttackofEnmey(), field_->get_hierarchy());
	effectManager_->Init();

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	playerUI_ = std::make_unique<PlayerUI>();

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

	vignetteNode = std::make_unique<VignetteNode>();
	vignetteNode->initialize();
	vignetteNode->set_texture_resource(outlineNode->result_stv_handle());
	vignetteNode->set_render_target();
	//vignetteNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());

	spriteNode = std::make_unique<SpriteNode>();
	spriteNode->initialize();
	//spriteNode->set_background_texture(outlineNode->result_stv_handle());
	spriteNode->set_background_texture(vignetteNode->result_stv_handle());
	spriteNode->set_render_target();
	//spriteNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());

	chromaticAberrationNode = std::make_unique<ChromaticAberrationNode>();
	chromaticAberrationNode->initialize();
	chromaticAberrationNode->set_render_target_SC(DirectXSwapChain::GetRenderTarget());
	chromaticAberrationNode->set_texture_resource(spriteNode->result_stv_handle());

	DirectXSwapChain::GetRenderTarget()->set_depth_stencil(nullptr);

	path.initialize({ object3DNode, outlineNode, vignetteNode , spriteNode, chromaticAberrationNode });
	RenderPathManager::RegisterPath("GameScene", std::move(path));
	RenderPathManager::SetPath("GameScene");

	posteffectManager = std::make_unique<PostEffectManager>();
	posteffectManager->initialize(
		vignetteNode, chromaticAberrationNode
	);
	posteffectManager->set_boss(boss_.get());
	posteffectManager->set_player(player_.get());

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	fadePanel_ = std::make_unique<FadePanel>();
	fadePanel_->SetFadeFadeStart(FadeType::Fade_Out);

	game_BGM_ = std::make_unique<AudioPlayer>();
	game_BGM_->initialize("meteOnigiri_gameBGM.wav", 0.5f, true);

#ifdef _DEBUG
	editor = CreateUnique<EditorController>();
	editor->initialize(camera3D_.get(), meteoriteManager_.get(), enemyManager_.get());
	meteoriteManager_->SetEditor(editor.get());
#endif // _DEBUG

}

void GameScene::load() {
	TextureManager::RegisterLoadQue("./Game/Resources/UI", "Fade_Panel.png");

	PolygonMeshManager::RegisterLoadQue("./Engine/Resources", "Planet.obj");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources", "player.obj");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources", "particle.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/Models", "mouth.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/Models", "mob.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/Models", "Field.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/Models", "kariEnemy.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/Models", "kariSpEnemy.obj");

	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/PlayerBody", "playerBody.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/PlayerArm", "playerArm.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/RiceScoop", "riceScoop.obj");

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
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/Skydome", "skydome.obj");

	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/FullStomach", "fullStomach.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/FullStomach", "fullStomach0.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/FullStomach", "fullStomach1.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/FullStomach", "fullStomach2.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/FullStomach", "fullStomach3.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/FullStomach", "fullStomach4.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/FullStomach", "fullStomach5.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/FullStomach", "fullStomach6.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/FullStomach", "fullStomach7.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/FullStomach", "fullStomach8.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/FullStomach", "fullStomach9.obj");
	PolygonMeshManager::RegisterLoadQue("./Game/Resources/GameScene/FullStomach", "fullStomachPercent.obj");

	PolygonMeshManager::RegisterLoadQue("./Game/Resources/Particle", "kometubu.obj");

	TextureManager::RegisterLoadQue("./Game/Resources/UI", "UI1.png");
	TextureManager::RegisterLoadQue("./Game/Resources/UI", "UI2.png");

	AudioManager::RegisterLoadQue("./Game/Resources/Audio/game", "SE_brap.wav");
	AudioManager::RegisterLoadQue("./Game/Resources/Audio/game", "SE_enemyAtract.wav");//
	AudioManager::RegisterLoadQue("./Game/Resources/Audio/game", "SE_meteoEachOther.wav");//
	AudioManager::RegisterLoadQue("./Game/Resources/Audio/game", "SE_bossHited.wav");//
	AudioManager::RegisterLoadQue("./Game/Resources/Audio/game", "SE_enemyAttack.wav");//
	AudioManager::RegisterLoadQue("./Game/Resources/Audio/game", "SE_enemyHitToMeteo.wav");//
	AudioManager::RegisterLoadQue("./Game/Resources/Audio/game", "SE_fieldPush.wav");
	AudioManager::RegisterLoadQue("./Game/Resources/Audio/game", "SE_playerKick.wav");//
	AudioManager::RegisterLoadQue("./Game/Resources/Audio/game", "SE_enemyEachOther.wav");//
	AudioManager::RegisterLoadQue("./Game/Resources/Audio/game", "meteOnigiri_gameBGM.wav");//
	AudioManager::RegisterLoadQue("./Game/Resources/Audio/game", "SE_bossAppearace.wav");//

}

void GameScene::begin() {
}

void GameScene::update() {
	camera3D_->update();
	Camera2D::CameraUpdate();

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	if (!camera3D_->isActiveDebugCamera()) {
		playerUI_->Update(player_->world_position(), camera3D_->vp_matrix(), player_->GetIsAttack());
	}

	fadePanel_->Update();

	if (!fadePanel_->GetIsFadeFinish()) {
		return;
	}

	// -------------------------------------------------
	// ↓ Inputの更新
	// -------------------------------------------------
	Input::GetInstance()->Update();

	AdjustmentItem::GetInstance()->Update();

	effectManager_->Update();

	// -------------------------------------------------
	// ↓ 演出が始まるかどうか
	// -------------------------------------------------
	if (performanceType_ != PerformanceType::None_Type) {
		switch (performanceType_) {
		case PerformanceType::GameStart_Type:
			GameStartPerformance();
			// 一定以上カメラが動いたら
			if (camera3D_->GetNowIndex() >= 3) {
				boss_->FaceSet();
				// 顔を上げるのが終わったら
				if (boss_->GetIsFinish()) {
					performanceType_ = PerformanceType::None_Type;
					boss_->SetIsDrawOverLine(true);
					boss_->SetIsStart(true);
					camera3D_->SetIsPerformanceFinish(false);
					enemyManager_->StartPop();
					meteoriteManager_->StartPop();
					game_BGM_->play();
				}
			}

			break;
		
		case PerformanceType::GameOver_Type:
			if (!doneEndAnimationPerformance) {
				GameOverPerformance();
			}
			// 終わったらフェードアウトする


			break;

		case PerformanceType::GameClear_Type:
			if (!doneEndAnimationPerformance) {
				GameClearPerformance();
			}
			// 終わったらフェードアウトする
			break;
		}

		return;
	}

	// -------------------------------------------------
	// ↓ ゲームクリア/オーバー確認
	// -------------------------------------------------
	if (boss_->GetIsClear()) {
		boss_->SetIsDrawOverLine(false);
		boss_->SetIsStart(false);
		game_BGM_->stop();
		performanceType_ = PerformanceType::GameClear_Type;
	}

	if (boss_->GetIsGameOver(field_->GetCylinderHight())) {
		boss_->SetIsDrawOverLine(false);
		boss_->SetIsStart(false);
		game_BGM_->stop();
		performanceType_ = PerformanceType::GameOver_Type;
	}

	// -------------------------------------------------
	// ↓ GameObjectの更新
	// -------------------------------------------------
	skydome_->update();
	field_->Update();
	satisFaction_->Update(boss_->GetSatietyLevel(), boss_->GetSatietyLevelLimit());

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

	effectManager_->Update();

	posteffectManager->update();

	// -------------------------------------------------
	// ↓ 当たり判定系
	// -------------------------------------------------
	
	CheckMeteoAttraction();

	CheckBossCollision();

	CheckMeteoToField();

	// -------------------------------------------------
	// ↓ UI
	// -------------------------------------------------
}

void GameScene::begin_rendering() {

	camera3D_->begin_rendering(*camera3D_);
	camera3D_->update_matrix();

	skydome_->begin_rendering(*camera3D_);
	field_->begin_rendering(*camera3D_);
	satisFaction_->BeginRendering(camera3D_.get());
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

	fadePanel_->Begin_Rendering();
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
	skydome_->draw();
	if (performanceType_ == PerformanceType::None_Type){
		satisFaction_->Draw();
	}
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
	editor->draw_debug3d();
	if (isDrawCollider_) {
		collisionManager_->debug_draw3d(*camera3D_);
	}
#endif
	RenderPathManager::Next();
	outlineNode->draw();
	RenderPathManager::Next();
	vignetteNode->draw();
	RenderPathManager::Next();
	if (performanceType_ == PerformanceType::None_Type) {
		playerUI_->Draw();
	}

	fadePanel_->Draw();
	RenderPathManager::Next();
	chromaticAberrationNode->draw();
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
				boss_->OnCollision(meteo->GetRadius());
				boss_->PlayFieldPushSE();
			}
		}
	}
}

void GameScene::CheckMeteoAttraction() {
	for (std::unique_ptr<Meteorite>& meteo : meteoriteList_) {
		if (!player_->GetIsAttack()) {
			meteo->SetIsAttraction(false);
			continue;
		}
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
			} else {
				meteo->SetAcceleration(Vector3::Normalize(meteoToAttractEnd));
				length = endLength;
				direction = Vector3::Normalize(meteoToAttractEnd);
				meteo->SetTargetPosition(player_->GetGravityRodEnd());
			}
		} else {
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
				boss_->OnCollision(meteo->GetRadius());
			}
		}
	}
}

void GameScene::GameStartPerformance() {
	if (!camera3D_->GetIsPerformanceFinish()) {
		camera3D_->GameStartPerformance();
	}

	if (camera3D_->GetIsStop()) {
		boss_->FaceShake();

		if (boss_->GetIsFinish()) {
			camera3D_->SetIsStop(false);
		}
	}
}

void GameScene::GameOverPerformance() {
	if (!camera3D_->GetIsPerformanceFinish()) {
		boss_->MouthClose();
		camera3D_->GameOverPerformance();
	} else {
		doneEndAnimationPerformance = true;
		// ゲームオーバーの終了
		fadePanel_->SetFadeFadeStart(FadeType::Fade_In);
		SceneManager::SetSceneChange(CreateUnique<GameOverScene>(),
									 static_cast<float>((fadePanel_->GetFadeTime() + 10) * GameTimer::DeltaTime()),
									 false);
	}
}

void GameScene::GameClearPerformance() {
	if (!camera3D_->GetIsPerformanceFinish()) {
		boss_->GameClearFaceSet();
		camera3D_->GameClearPerformance();
	} else {
		doneEndAnimationPerformance = true;
		boss_->Burp();	// げっぷをする
		fadePanel_->SetFadeFadeStart(FadeType::Fade_In);
		SceneManager::SetSceneChange(CreateUnique<ClearScene>(),
									 static_cast<float>((fadePanel_->GetFadeTime() + 10) * GameTimer::DeltaTime()),
									 false);
	}
}

#ifdef _DEBUG

#include <externals/imgui/imgui.h>

void GameScene::debug_update() {
	ImGui::Begin("Camera3D");
	if(ImGui::Button("restart")) {
		camera3D_->Restart();
	}
	camera3D_->debug_gui();
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

	ImGui::Begin("Collider");
	ImGui::Checkbox("isDraw", &isDrawCollider_);
	ImGui::End();

	meteoriteManager_->DebugGui();

	enemyManager_->EditImGui();

	posteffectManager->debug_gui();

	satisFaction_->Debug_gui();
}
#endif // _DEBUG
