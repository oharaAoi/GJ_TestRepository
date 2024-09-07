#include "GameScene.h"

#include "Engine/Game/Managers/SceneManager/SceneManager.h"
#include "Engine/Utility/Utility.h"
#include "Engine/Render/RenderPathManager/RenderPathManager.h"
#include "Engine/DirectX/DirectXCore.h"

void GameScene::initialize() {
	Input::GetInstance()->Init(WinApp::GetWNDCLASS(), WinApp::GetWndHandle());
	EffectManager::GetInstance()->Init();
	effectManager_ = EffectManager::GetInstance();

	field_ = std::make_unique<Field>();
	player_ = std::make_unique<Player>();

	camera3D_ = std::make_unique<FollowCamera>();
	camera3D_->initialize();
	camera3D_->set_transform({
		CVector3::BASIS,
		Quaternion::EulerDegree(90, 0, 0),
		{ 0, 35, 0 }
	});
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
	player_->Update();

	frameCount_++;

	if (frameCount_ > 240) {
		AddMeteoriteList();
		frameCount_ = 0;
	}

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

	// -------------------------------------------------
	// ↓ 当たり判定系
	// -------------------------------------------------
	if (player_->GetIsAttack()) {
		CheckMeteoAttraction();
	}
}

void GameScene::begin_rendering() {
	field_->begin_rendering(*camera3D_);
	
	player_->Begin_Rendering(camera3D_.get());

	for (Meteorite& meteo : meteoriteList_) {
		meteo.begin_rendering(*camera3D_);
	}

	effectManager_->BeginRendering(*camera3D_);
}

void GameScene::late_update() {

}

void GameScene::draw() const {
	RenderPathManager::BeginFrame();
	DirectXCore::ShowGrid(*camera3D_);
	field_->draw();
	player_->Draw();

	for (const Meteorite& meteo : meteoriteList_) {
		meteo.draw();
	}

	effectManager_->Draw();

	RenderPathManager::Next();
}


void GameScene::AddMeteoriteList() {
	meteoriteList_.emplace_back();
}


void GameScene::CheckMeteoAttraction() {
	for (Meteorite& meteo : meteoriteList_) {
		Vector3 meteoToRodOri = (meteo.get_transform().get_translate() - player_->GetGravityRodOrigine());
		Vector3 projectVector = Vector3::Projection(meteoToRodOri, player_->GetGravityRodVector());
		// 最近接点を求める
		Vector3 closesPoint = player_->GetGravityRodOrigine() + projectVector;
		// 最近接点と隕石の距離を求める
		float length = Vector3::Length(meteo.get_transform().get_translate(), closesPoint);

		// 2点間の距離が重力圏の範囲と隕石の半径の合計より短かったら
		if (length < player_->GetGravityRod()->GetAttractionRange() + meteo.GetRadius()) {
			meteo.SetIsAttraction(true);
			meteo.SetAcceleration(Vector3::Normalize(closesPoint - meteo.get_transform().get_translate()));

			// 一応死亡判定を取って置く
			if (length < meteo.GetRadius()) {
				meteo.SetIsDead(true);
				effectManager_->AddEffect("default", meteo.get_transform().get_translate(), 
										  Vector3::Normalize(meteo.get_transform().get_translate() - closesPoint));
			}

		} else {
			meteo.SetIsAttraction(false);
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

	ImGui::Begin("Meteorite");
	ImGui::DragFloat("attractionedStrength", &Meteorite::attractionedStrength_, 0.1f);
	ImGui::DragFloat("kSpeed", &Meteorite::kSpeed_, 0.1f);
	ImGui::DragFloat("radius", &Meteorite::radius_, 0.1f);
	ImGui::End();
}
#endif // _DEBUG
