#include "TutorialScene.h"
#include "Engine/Game/Managers/SceneManager/SceneManager.h"
#include "Engine/Game/Managers/TextureManager/TextureManager.h"
#include "Engine/Render/RenderTargetGroup/SwapChainRenderTargetGroup.h"

void TutorialScene::finalize() {
	object3DNode->finalize();
	outlineNode->finalize();
	spriteNode->finalize();
	RenderPathManager::UnregisterPath("TutorialScene");
}

void TutorialScene::initialize() {
	input_ = Input::GetInstance();

	AdjustmentItem::GetInstance()->Init("TutorialScene");
	collisionManager_ = std::make_unique<CollisionManager>();

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	Camera2D::Initialize();
	camera3D_ = std::make_unique<Camera3D>();
	camera3D_->initialize();
	camera3D_->set_transform({
		CVector3::BASIS,
		Quaternion::EulerDegree(55, 0, 0),
		{ 0, 50, -28.0 }
							 });

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	field_ = std::make_unique<Field>();
	skydome_ = std::make_unique<GameObject>();
	skydome_->reset_object("skydome.obj");

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	player_ = std::make_unique<Player>();
	player_->set_parent(field_->get_hierarchy());
	boss_ = std::make_unique<Boss>();
	boss_->get_transform().set_translate_y(-2.0f);

	// -------------------------------------------------
	// ↓ 
	// -------------------------------------------------
	collisionManager_->register_collider("Player", player_->GetCollider());
	meteoriteManager_ = std::make_unique<MeteoriteManager>(meteoriteList_, collisionManager_.get());
	enemyManager_ = std::make_unique<EnemyManager>(enemyList_, collisionManager_.get(), player_->GetIsAttackofEnmey(), field_->get_hierarchy());

	// ---------------------------------------------
	tutorialUI_ = std::make_unique<TutorialUI>();

	// ---------------------------------------------
	isTutorialFinish_ = false;
	content_ = TutorialContent::FirstMove_Content;
	frameCount_ = 0;
	// ---------------------------------------------

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
	RenderPathManager::RegisterPath("TutorialScene", std::move(path));
	RenderPathManager::SetPath("TutorialScene");

	fadePanel_ = std::make_unique<FadePanel>();
	fadePanel_->SetFadeFadeStart(FadeType::Fade_Out);
}

void TutorialScene::load() {
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources", "particle.obj");

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

	TextureManager::RegisterLoadQue("./Game/Resources/UI", "UI_PlayerControl_move.png");
	TextureManager::RegisterLoadQue("./Game/Resources/UI", "UI_PlayerControl_attack.png");
	TextureManager::RegisterLoadQue("./Game/Resources/UI", "UI_kari.png");

	// content
	TextureManager::RegisterLoadQue("./Game/Resources/TutorialScene", "appearance.png");
	TextureManager::RegisterLoadQue("./Game/Resources/TutorialScene", "attract.png");
	TextureManager::RegisterLoadQue("./Game/Resources/TutorialScene", "attractEnemy.png");
	TextureManager::RegisterLoadQue("./Game/Resources/TutorialScene", "collision.png");
	TextureManager::RegisterLoadQue("./Game/Resources/TutorialScene", "collisionEnemy.png");
	TextureManager::RegisterLoadQue("./Game/Resources/TutorialScene", "complete.png");
	TextureManager::RegisterLoadQue("./Game/Resources/TutorialScene", "fall.png");
	TextureManager::RegisterLoadQue("./Game/Resources/TutorialScene", "kick.png");
	TextureManager::RegisterLoadQue("./Game/Resources/TutorialScene", "move.png");
	TextureManager::RegisterLoadQue("./Game/Resources/TutorialScene", "practiceRange.png");
	TextureManager::RegisterLoadQue("./Game/Resources/TutorialScene", "putAway.png");
	TextureManager::RegisterLoadQue("./Game/Resources/TutorialScene", "putOut.png");
	TextureManager::RegisterLoadQue("./Game/Resources/TutorialScene", "rotate.png");
	TextureManager::RegisterLoadQue("./Game/Resources/TutorialScene", "scrollEnd.png");
	TextureManager::RegisterLoadQue("./Game/Resources/TutorialScene", "scrollStart.png");
	TextureManager::RegisterLoadQue("./Game/Resources/TutorialScene", "tips.png");
	TextureManager::RegisterLoadQue("./Game/Resources/TutorialScene", "tutorial.png");
	TextureManager::RegisterLoadQue("./Game/Resources/TutorialScene", "woodBoard.png");

	AudioManager::RegisterLoadQue("./Game/Resources/Audio", "SE_brap.wav");
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

	// -------------------------------------------------
	// ↓ 次のシーンに行くかの判定
	// -------------------------------------------------
	if (input_->GetIsPadPress(XINPUT_GAMEPAD_A)) {
		if (++skipCount_ >= 120) {
			fadePanel_->SetFadeFadeStart(FadeType::Fade_In);
			SceneManager::SetSceneChange(CreateUnique<GameScene>(),
										 static_cast<float>((fadePanel_->GetFadeTime() + 10) * GameTimer::DeltaTime()),
										 false);
		}
	}

	// -------------------------------------------------
	// ↓ カメラを更新
	// -------------------------------------------------
	if (!isTutorialFinish_) {
		field_->Update();
		ExecuteTutorialContent(content_);
		return;
	}

	// -------------------------------------------------
	// ↓ GameObjectの更新
	// -------------------------------------------------
	skydome_->update();
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
	if (player_->GetIsAttack()) {
		CheckMeteoAttraction();
	} else {
		for (std::unique_ptr<Meteorite>& meteo : meteoriteList_) {
			meteo->SetIsAttraction(false);
		}
	}
	CheckBossCollision();
	CheckMeteoToField();

	// -------------------------------------------------
	// ↓ UI
	// -------------------------------------------------

	tutorialUI_->Update();
}

void TutorialScene::begin_rendering() {
	camera3D_->begin_rendering(*camera3D_);
	camera3D_->update_matrix();

	skydome_->begin_rendering(*camera3D_);
	field_->begin_rendering(*camera3D_);

	player_->Begin_Rendering(camera3D_.get());
	boss_->Begin_Rendering(camera3D_.get());

	for (std::unique_ptr<Meteorite>& meteo : meteoriteList_) {
		meteo->begin_rendering(*camera3D_);
	}

	for (std::unique_ptr<Enemy>& enemy : enemyList_) {
		enemy->begin_rendering(*camera3D_);
	}

	tutorialUI_->BeginRendering();

	fadePanel_->Begin_Rendering();
}

void TutorialScene::late_update() {
	collisionManager_->update();

	// Enemy同士 ---------------------------------------------------
	collisionManager_->collision("Enemy", "Enemy");
	// Enemyと隕石 -------------------------------------------------
	collisionManager_->collision("Enemy", "Meteo");
	// 敵とPlayer --------------------------------------------------
	collisionManager_->collision("Enemy", "Player");
	// メテオ同士 ---------------------------------------------------
	collisionManager_->collision("Meteo", "Meteo");
}

void TutorialScene::draw() const {
	RenderPathManager::BeginFrame();
	skydome_->draw();
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
	outlineNode->draw();
	RenderPathManager::Next();
	tutorialUI_->Draw();
	fadePanel_->Draw();
	RenderPathManager::Next();
}


#ifdef _DEBUG
#include <externals/imgui/imgui.h>
void TutorialScene::debug_update() {
	ImGui::Begin("Tutorial");
	ImGui::Text("nowScene: Tutorial");
	ImGui::Text("nextScene: Game");
	ImGui::Text("Press: A");
	ImGui::Text("nextOfFrame: %d / 120", frameCount_);
	ImGui::Separator();
	ImGui::Text("now content :");
	ImGui::SameLine();
	switch (content_) {
	case TutorialContent::FirstMove_Content:
		ImGui::Text("FirstMove_Content");
		break;
	case TutorialContent::RodPutOn_Content:
		ImGui::Text("RodPutOn_Content:");
		break;
	case TutorialContent::MeteoCollision_Content:
		ImGui::Text("MeteoCollision_Content");
		break;
	case TutorialContent::CantMoveCanRotate_Content:
		ImGui::Text("CantMoveCanRotate_Content");
		break;
	case TutorialContent::FirstEnemy_Content:
		ImGui::Text("FirstEnemy_Content");
		break;
	case TutorialContent::EnemyCollisionToMeteo_Content:
		ImGui::Text("EnemyCollisionToMeteo_Content");
		break;
	case TutorialContent::MeteoAttract_Content:
		ImGui::Text("MeteoAttract_Content");
		break;
	}
	ImGui::End();

	ImGui::Begin("Camera3D");
	camera3D_->debug_gui();
	ImGui::End();

	ImGui::Begin("Field");
	field_->EditImGui();
	ImGui::End();

	ImGui::Begin("UI");
	tutorialUI_->EditGui();
	ImGui::End();

	boss_->EditImGui();

	enemyManager_->EditImGui();
}
#endif // _DEBUG

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

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialScene::FirstMoveContent() {
	// Playerの移動のみを教える
	// 速度を取得してFrameを数える
	player_->Update(field_->GetRadius());

	player_->SetIsAttack(false);

	Vector3 velocity = player_->GetVelocity();

	if (std::abs(velocity.x) > 0.4f || std::abs(velocity.z) > 0.4f) {
		++frameCount_;
	}

	if (frameCount_ > 100) {
		content_ = TutorialContent::RodPutOn_Content;
		frameCount_ = 0;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialScene::RodPutOnContent() {
	// 棒を取り出したら終わり
	player_->Update(field_->GetRadius());

	if (player_->GetIsAttack()) {
		content_ = TutorialContent::MeteoCollision_Content;
		Vector3 playerTranslate = player_->GetGravityRodOrigine() + Vector3{10, 0, 0};
		meteoriteManager_->AddMeteo(player_->GetGravityRodEnd() + Vector3{ 10, 0, 0 });
		meteoriteManager_->AddMeteo(player_->GetGravityRodEnd() + Vector3{ 15, 0, 0 });
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialScene::MeteoCollisionContent() {
	// 隕石同士がぶつかることを教える
	player_->SetIsAttack(true);

	for (std::unique_ptr<Meteorite>& meteo : meteoriteList_) {
		meteo->Update(player_->get_transform().get_translate());
		meteo->get_transform().set_translate_z(player_->GetGravityRodOrigine().z);
	}

	// 死亡フラグのチェックを行う
	meteoriteList_.remove_if([](const std::unique_ptr<Meteorite>& meteo) {
		if (meteo->GetIsDead()) {
			return true;
		}
		return false;
	});

	if (player_->GetIsAttack()) { CheckMeteoAttraction(); }
	CheckMeteoToField();
	CheckBossCollision();

	if (meteoriteList_.size() == 0) {
		content_ = TutorialContent::CantMoveCanRotate_Content;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialScene::CantMoveCanRotateContent() {
	// Playerがスティックを回していたら
	player_->Update(field_->GetRadius());
	player_->SetIsAttack(true);

	Vector3 velocity = player_->GetVelocity();
	if (std::abs(velocity.x) > 0.4f || std::abs(velocity.z) > 0.4f) {
		++frameCount_;
	}

	if (frameCount_ > 80) {
		content_ = TutorialContent::FirstEnemy_Content;
		frameCount_ = 0;
		enemyManager_->AddEnemy(player_->get_transform().get_translate() + Vector3{2.0f, 0.0f, 2.0f}, EnemyType::Normal_Type);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialScene::FirstEnemyContent() {
	// 敵を蹴れることを教える
	player_->Update(field_->GetRadius());

	for (std::unique_ptr<Enemy>& enemy : enemyList_) {
		enemy->Update(player_->get_transform().get_translate());

		if (enemy->GetIsKickToPlayer()) {
			// 速度が0になったら
			if (enemy->GetVelocity() == Vector3{ 0,0,0 }) {
				content_ = TutorialContent::EnemyCollisionToMeteo_Content;
				meteoriteManager_->AddMeteo(Vector3{20, 0, enemy->get_transform().get_translate().z});
			}
		}
	}

	enemyList_.remove_if([](const std::unique_ptr<Enemy>& enemy) {
	if (enemy->GetIsDead()) {
		return true;
	}
	return false;
	 });

}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialScene::EnemyCollisionToMeteoContent() {
	player_->Update(field_->GetRadius());

	// 隕石と敵をぶつける
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
		Quaternion rotation = enemy->get_transform().get_quaternion();
		Quaternion rotateValue = Quaternion::AngleAxis(enemy->GetRotateAxis(),4.0f * ToRadian);
		enemy->get_transform().set_rotate(rotateValue * rotation);
	}

	enemyList_.remove_if([](const std::unique_ptr<Enemy>& enemy) {
		if (enemy->GetIsDead()) {
			return true;
		}
		return false;
						 });

	if (enemyList_.size() == 0) {
		content_ = TutorialContent::MeteoAttract_Content;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialScene::MeteoAttractContent() {
	// 隕石を引き寄せられることを教える
	player_->Update(field_->GetRadius());

	// 無視した場合を考慮
	if (++frameCount_ > 200) {
		meteoriteManager_->AddMeteo(Vector3{ 20, 0, RandomFloat(-3, 5)});
		frameCount_ = 0;
	}

	for (std::unique_ptr<Meteorite>& meteo : meteoriteList_) {
		meteo->Update(player_->get_transform().get_translate());
		if (meteo->GetIsAttraction()) {
			isTutorialFinish_ = true;
			enemyManager_->StartPop();
			meteoriteManager_->StartPop();
			frameCount_ = 0;
		}
	}

	// 死亡フラグのチェックを行う
	meteoriteList_.remove_if([](const std::unique_ptr<Meteorite>& meteo) {
		if (meteo->GetIsDead()) {
			return true;
		}
		return false;
	 });

	if (player_->GetIsAttack()) { CheckMeteoAttraction(); }
	CheckMeteoToField();
	CheckBossCollision();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　
//////////////////////////////////////////////////////////////////////////////////////////////////

void TutorialScene::ExecuteTutorialContent(const TutorialContent& content) {
	const auto& it = functionMap_.find(content);
	if (it != functionMap_.end()) {
		(this->*(it->second))(); // メンバー関数ポインタを使って関数を呼び出す
	} 
}
