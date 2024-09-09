#include "GameScene.h"

#include "Engine/Game/Managers/SceneManager/SceneManager.h"
#include "Engine/Utility/Utility.h"
#include "Engine/Render/RenderPathManager/RenderPathManager.h"
#include "Engine/DirectX/DirectXCore.h"

void GameScene::initialize() {
	Input::GetInstance()->Init(WinApp::GetWNDCLASS(), WinApp::GetWndHandle());
	EffectManager::GetInstance()->Init();
	effectManager_ = EffectManager::GetInstance();
	collisionManager_ = std::make_unique<CollisionManager>();

	field_ = std::make_unique<Field>();
	player_ = std::make_unique<Player>();
	boss_ = std::make_unique<Boss>();

	camera3D_ = std::make_unique<FollowCamera>();
	camera3D_->initialize();
	camera3D_->set_transform({
		CVector3::BASIS,
		Quaternion::EulerDegree(55, 0, 0),
		{ 0, 50, -28.0 }
							 });

	meteoriteManager_ = std::make_unique<MeteoriteManager>();
	meteoriteManager_->SetGameScene(this);
	meteoriteManager_->Init();

	enemyManager_ = std::make_unique<EnemyManager>(this);

	collisionManager_->register_collider("Player", player_->GetCollider());
}

void GameScene::load() {
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources", "Planet.obj");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources", "player.obj");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources", "particle.obj");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources/Models", "GravityRod.obj");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources/Models", "mouth.obj");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources/Models", "mob.obj");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources/Models", "Field.obj");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources/Models", "kariEnemy.obj");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources/Models", "kariSpEnemy.obj");
}

void GameScene::begin() {
}

void GameScene::update() {

	// -------------------------------------------------
	// ↓ Inputの更新
	// -------------------------------------------------
	Input::GetInstance()->Update();

	// -------------------------------------------------
	// ↓ カメラを更新
	// -------------------------------------------------
	camera3D_->update();
	camera3D_->update_matrix();

	// -------------------------------------------------
	// ↓ GameObjectの更新
	// -------------------------------------------------
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
}

void GameScene::begin_rendering() {
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
}

void GameScene::late_update() {
	collisionManager_->update();

	
	for (std::unique_ptr<Enemy>& enemy : enemyList_) {
		enemy->SetNextCollision(0);
		if (enemy->GetIsAttack()) {
			player_->SetIsAttackofEnmey(true);
		} else {
			player_->SetIsAttackofEnmey(false);
		}
		collisionManager_->collision("Player", enemy->GetMeteoId());
	}

	// メテオ同士
	std::list<std::unique_ptr<Meteorite>>::iterator iterA = meteoriteList_.begin();
	for (; iterA != meteoriteList_.end(); ++iterA) {
		Meteorite* meteoA = iterA->get();
		std::list<std::unique_ptr<Meteorite>>::iterator iterB = iterA;
		iterB++;
		for (; iterB != meteoriteList_.end(); ++iterB) {
			Meteorite* meteoB = iterB->get();
			meteoA->SetNextCollision(0);
			meteoB->SetNextCollision(0);
			collisionManager_->collision(meteoA->GetMeteoId(), meteoB->GetMeteoId());
		}
	}

	// Enemy同士
	std::list<std::unique_ptr<Enemy>>::iterator eIterA = enemyList_.begin();
	for (; eIterA != enemyList_.end(); ++eIterA) {
		Enemy* enemyA = eIterA->get();
		std::list<std::unique_ptr<Enemy>>::iterator eIterB = eIterA;
		eIterB++;
		for (; eIterB != enemyList_.end(); ++eIterB) {
			Enemy* enemyB = eIterB->get();
			enemyA->SetNextCollision(2);
			enemyB->SetNextCollision(2);
			collisionManager_->collision(enemyA->GetMeteoId(), enemyB->GetMeteoId());
		}
	}

	// Enemyと隕石
	for (std::unique_ptr<Meteorite>& meteo : meteoriteList_) {
		for (std::unique_ptr<Enemy>& enemy : enemyList_) {
			enemy->SetNextCollision(1);
			meteo->SetNextCollision(1);
			collisionManager_->collision(meteo->GetMeteoId(), enemy->GetMeteoId());
		}
	}
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
#endif
	RenderPathManager::Next();
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

void GameScene::AddMeteorite(const Vector3& position) {
	auto& newMeteo = meteoriteList_.emplace_back(std::make_unique<Meteorite>(position));
	collisionManager_->register_collider(newMeteo->GetMeteoId(), newMeteo->GetCollider());
}

void GameScene::AddEnemy(const Vector3& position, const EnemyType& enemyType) {
	auto& newEnemy = enemyList_.emplace_back(std::make_unique<Enemy>(position, enemyType));
	collisionManager_->register_collider(newEnemy->GetMeteoId(), newEnemy->GetCollider());
}

void GameScene::CheckMeteoCollision() {
	//std::list<std::unique_ptr<Meteorite>>::iterator iterA = meteoriteList_.begin();
	//for (; iterA != meteoriteList_.end(); ++iterA) {
	//	Meteorite* meteoA = *iterA;

	//	std::list<std::unique_ptr<Meteorite>>::iterator iterB = iterA;
	//	iterB++;

	//	for (; iterB != meteoriteList_.end(); ++iterB) {
	//		Meteorite* meteoB = *iter);

	//		//float length = Vector3::Length(meteoA->world_position() - meteoB->world_position());
	//		float length = Vector3::Length(meteoA->get_transform().get_translate() - meteoB->get_transform().get_translate());

	//		if (length < meteoA->GetRadius() + meteoB->GetRadius()) {
	//			meteoA->OnCollision(meteoB->get_transform().get_translate());
	//			meteoB->OnCollision(meteoA->get_transform().get_translate());
	//		}
	//	}
	//}
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

void GameScene::CheckEnemyCollison() {
	// -------------------------------------------------
	// ↓ playerと敵
	// -------------------------------------------------
	for (std::unique_ptr<Enemy>& enemy : enemyList_) {
		if (!enemy->GetIsAttack()) {
			float length = Vector3::Length(player_->get_transform().get_translate() - enemy->get_transform().get_translate());

			if (length < player_->GetRadius() + enemy->GetRadius()) {
				enemy->OnCollision(player_->get_transform().get_translate(), 0);
			}
		}
	}

	// -------------------------------------------------
	// ↓ 隕石と敵
	// -------------------------------------------------
	/*for (std::unique_ptr<Enemy>& enemy : enemyList_) {
		if (!enemy->GetIsAttack()) {
			for (Meteorite& meteo : meteoriteList_) {
				float length = Vector3::Length(meteo.get_transform().get_translate() - enemy->get_transform().get_translate());
				if (length < meteo.GetRadius() + enemy->GetRadius()) {
					enemy->OnCollision(meteo.get_transform().get_translate(), 1);
					meteo.SetIsEnemyHit(true);
				}
			}
		}
	}*/

	// -------------------------------------------------
	// ↓ 敵同士
	// -------------------------------------------------
	//std::list<std::unique_ptr<Enemy>>::iterator iterA = enemyList_.begin();
	//for (; iterA != enemyList_.end(); ++iterA) {
	//	Enemy* enemyA = (*iterA).get();

	//	std::list<std::unique_ptr<Enemy >>::iterator iterB = iterA;
	//	iterB++;

	//	for (; iterB != enemyList_.end(); ++iterB) {
	//		Enemy* enemyB = (*iterB).get();
	//		float length = Vector3::Length(enemyA->get_transform().get_translate() - enemyB->get_transform().get_translate());

	//		if (length < enemyA->GetRadius() + enemyB->GetRadius()) {
	//			enemyA->OnCollision(enemyB->get_transform().get_translate(), 2);
	//			enemyB->OnCollision(enemyA->get_transform().get_translate(), 2);

	//			/*Vector3 pos = RandomVector3(-4.0f, 4.0f);
	//			AddEnemy(pos, EnemyType::SpecialPop_Type);*/
	//		}
	//	}
	//}
}

#ifdef _DEBUG

#include <externals/imgui/imgui.h>

void GameScene::debug_update() {
	ImGui::Begin("Camera3D");
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

	meteoriteManager_->EditImGui();
	enemyManager_->EditImGui();
}
#endif // _DEBUG
