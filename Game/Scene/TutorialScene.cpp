#include "TutorialScene.h"
#include "Engine/Game/Managers/SceneManager/SceneManager.h"
#include "Engine/Game/Managers/TextureManager/TextureManager.h"

void TutorialScene::initialize() {
	input_ = Input::GetInstance();

	AdjustmentItem::GetInstance()->Init("TutorialScene");
	collisionManager_ = std::make_unique<CollisionManager>();

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

	collisionManager_->register_collider("Player", player_->GetCollider());
	meteoriteManager_ = std::make_unique<MeteoriteManager>(meteoriteList_, collisionManager_.get());
	enemyManager_ = std::make_unique<EnemyManager>(enemyList_, collisionManager_.get(), player_->GetIsAttackofEnmey());

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
	meteoriteManager_->Update(player_->get_transform().get_translate());
	enemyManager_->Update(player_->get_transform().get_translate());

	// -------------------------------------------------
	// ↓ 当たり判定系
	// -------------------------------------------------
	if (player_->GetIsAttack()) {CheckMeteoAttraction();}
	CheckBossCollision();
	CheckMeteoToField();
}

void TutorialScene::begin_rendering() {
	camera3D_->begin_rendering(*camera3D_);
	camera3D_->update_matrix();

	field_->begin_rendering(*camera3D_);
	player_->Begin_Rendering(camera3D_.get());
	boss_->Begin_Rendering(camera3D_.get());

	for (std::unique_ptr<Meteorite>& meteo : meteoriteList_) {
		meteo->begin_rendering(*camera3D_);
	}

	for (std::unique_ptr<Enemy>& enemy : enemyList_) {
		enemy->begin_rendering(*camera3D_);
	}
}

void TutorialScene::late_update() {
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

void TutorialScene::draw() const {
	RenderPathManager::BeginFrame();
	field_->draw();
	player_->Draw();
	boss_->Draw();

	for (const std::unique_ptr<Meteorite>& meteo : meteoriteList_) {
		meteo->draw();
	}
	for (const std::unique_ptr<Enemy>& enemy : enemyList_) {
		enemy->draw();
	}

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

	enemyManager_->EditImGui();
}
void TutorialScene::CheckMeteoToField() {
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

void TutorialScene::CheckMeteoAttraction() {
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
				meteo->SetAcceleration(meteoToAttractOrigine.normalize_safe());
				length = origineLength;
				direction = meteoToAttractOrigine.normalize_safe();
				meteo->SetTargetPosition(player_->GetGravityRodOrigine());
			} else {
				meteo->SetAcceleration(meteoToAttractEnd.normalize_safe());
				length = endLength;
				direction = meteoToAttractEnd.normalize_safe();
				meteo->SetTargetPosition(player_->GetGravityRodEnd());
			}
		} else {
			meteo->SetIsAttraction(false);
		}
	}
}

void TutorialScene::CheckBossCollision() {
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
#endif // _DEBUG