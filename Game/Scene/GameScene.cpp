#include "GameScene.h"

#include "Engine/Game/Managers/SceneManager/SceneManager.h"
#include "Engine/Utility/Utility.h"
#include "Engine/Render/RenderPathManager/RenderPathManager.h"
#include "Engine/DirectX/DirectXCore.h"

void GameScene::initialize() {
	Input::GetInstance()->Init(WinApp::GetWNDCLASS(), WinApp::GetWndHandle());
	EffectManager::GetInstance()->Init();
	effectManager_ = EffectManager::GetInstance();
	meteoriteManager_ = std::make_unique<MeteoriteManager>();

	field_ = std::make_unique<Field>();
	player_ = std::make_unique<Player>();
	boss_ = std::make_unique<Boss>();

	camera3D_ = std::make_unique<FollowCamera>();
	camera3D_->initialize();
	camera3D_->set_transform({
		CVector3::BASIS,
		Quaternion::EulerDegree(40, 0, 0),
		{ 0, 27, -25 }
							 });

	meteoriteManager_->SetGameScene(this);
	meteoriteManager_->Init();
}

void GameScene::load() {
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources", "Planet.obj");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources", "Field.obj");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources", "player.obj");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources", "particle.obj");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources/Models", "GravityRod.obj");
	PolygonMeshManager::RegisterLoadQue("./Engine/Resources/Models", "mouth.obj");
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

	for (Meteorite& meteo : meteoriteList_) {
		meteo.Update();
	}

	// 死亡フラグのチェックを行う
	meteoriteList_.remove_if([](const Meteorite& meteo) {
		if (meteo.GetIsDead()) {
			return true;
		}
		return false;
							 });

	// -------------------------------------------------
	// ↓ Manager系の更新
	// -------------------------------------------------
	effectManager_->Update();

	meteoriteManager_->Update();

	// -------------------------------------------------
	// ↓ 当たり判定系
	// -------------------------------------------------
	if (player_->GetIsAttack()) {
		CheckMeteoAttraction();
	}

	CheckCollision();
}

void GameScene::begin_rendering() {
	field_->begin_rendering(*camera3D_);

	player_->Begin_Rendering(camera3D_.get());
	boss_->begin_rendering(*camera3D_);

	for (Meteorite& meteo : meteoriteList_) {
		meteo.begin_rendering(*camera3D_);
	}

	effectManager_->BeginRendering(*camera3D_);
}

void GameScene::late_update() {

}

void GameScene::draw() const {
	RenderPathManager::BeginFrame();
	//DirectXCore::ShowGrid(*camera3D_);
	field_->draw();
	player_->Draw();
	boss_->draw();

	for (const Meteorite& meteo : meteoriteList_) {
		meteo.draw();
	}

	effectManager_->Draw();

	RenderPathManager::Next();
}

void GameScene::CheckMeteoAttraction() {
	for (Meteorite& meteo : meteoriteList_) {
		Vector3 direction{};
		float length = 0;
		// 引き寄せる2つの球体との距離を測る
		Vector3 meteoToAttractOrigine = player_->GetGravityRodOrigine() - meteo.world_position();
		Vector3 meteoToAttractEnd = player_->GetGravityRodEnd() - meteo.world_position();

		float origineLength = meteoToAttractOrigine.length();
		float endLength = meteoToAttractEnd.length();

		if (origineLength < player_->GetGravityRod()->GetAttractionRange() || endLength < player_->GetGravityRod()->GetAttractionRange()) {
			meteo.SetIsAttraction(true);
			// 距離を比較して近い方とのベクトルを加速度に加算する
			if (origineLength < endLength) {
				meteo.SetAcceleration(Vector3::Normalize(meteoToAttractOrigine));
				length = origineLength;
				direction = Vector3::Normalize(meteoToAttractOrigine);
			} else {
				meteo.SetAcceleration(Vector3::Normalize(meteoToAttractEnd));
				length = endLength;
				direction = Vector3::Normalize(meteoToAttractEnd);
			}
		} else {
			meteo.SetIsAttraction(false);
		}

		/*if (length < meteo.GetRadius()) {
			meteo.SetIsDead(true);
			effectManager_->AddEffect("default", meteo.get_transform().get_translate(), direction);
		}*/
	}

}

void GameScene::AddMeteorite(const Vector3& position) {
	meteoriteList_.emplace_back(position);
}

void GameScene::CheckCollision() {
	std::list<Meteorite>::iterator iterA = meteoriteList_.begin();
	for (; iterA != meteoriteList_.end(); ++iterA) {
		Meteorite* meteoA = &(*iterA);

		std::list<Meteorite>::iterator iterB = iterA;
		iterB++;

		for (; iterB != meteoriteList_.end(); ++iterB) {
			Meteorite* meteoB = &(*iterB);

			//float length = Vector3::Length(meteoA->world_position() - meteoB->world_position());
			float length = Vector3::Length(meteoA->get_transform().get_translate() - meteoB->get_transform().get_translate());

			if (length < meteoA->GetRadius() + meteoB->GetRadius()) {
				meteoA->OnCollision(meteoB->get_transform().get_translate());
				meteoB->OnCollision(meteoA->get_transform().get_translate());
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

	field_->EditImGui();
	ImGui::Begin("GameObject");
	player_->debug_gui();
	ImGui::End();

	ImGui::Begin("Boss");
	boss_->debug_gui();
	ImGui::End();

	ImGui::Begin("Meteorite");
	ImGui::DragFloat("attractionedStrength", &Meteorite::attractionedStrength_, 0.1f);
	ImGui::DragFloat("kSpeed", &Meteorite::kSpeed_, 0.1f);
	ImGui::DragFloat("radius", &Meteorite::radius_, 0.1f);
	ImGui::End();

	meteoriteManager_->EditImGui();
}
#endif // _DEBUG
