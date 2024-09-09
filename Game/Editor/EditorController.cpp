#ifdef _DEBUG
#include "EditorController.h"

#include <externals/imgui/imgui.h>

void EditorController::initialize(const Camera3D* camera_, MeteoriteManager* meteoManager) {
	isEdit = false;
	editState = EditState::None;
	enemyEditor = std::make_unique<EnemyPopEditor>();
	meteoriteEditor = std::make_unique<MeteoritePopEditor>();
	meteoriteEditor->initialize(meteoManager);

	isPlayDemo = false;

	camera = camera_;
}

void EditorController::draw_gui() {
	main_gui();
	if (!isEdit) {
		return;
	}
	// デモプレイ中はGUIを表示しない
	if (isPlayDemo) {
		return;
	}

	if (editState == EditState::Enemy) {
		enemyEditor->draw_editor();
	}
	if (editState == EditState::Meteorite) {
		meteoriteEditor->draw_editor();
	}
}

void EditorController::draw_debug3d() {
	if (!isEdit) {
		return;
	}
	// デモプレイ中はGUIを表示しない
	if (isPlayDemo) {
		return;
	}
	if (editState == EditState::Enemy) {
		enemyEditor->draw_editor();
	}
	if (editState == EditState::Meteorite) {
		meteoriteEditor->debug_draw3d(camera);
	}
}

bool EditorController::is_edit() const {
	return isEdit;
}

void EditorController::main_gui() {
	ImGui::Begin("EditorGUI");
	ImGui::Checkbox("IsEdit", &isEdit);
	// デモプレイ中は絶対にエディターを切れないようにする
	if (isPlayDemo) {
		isEdit = true;
	}
	// エディターを使用しない場合はここで早期return
	if (!isEdit) {
		ImGui::End();
		return;
	}
	// 各種GUIを描画するか
	if (!isPlayDemo) {
		if (ImGui::CollapsingHeader("EditState", ImGuiTreeNodeFlags_DefaultOpen)) {
			if (ImGui::RadioButton("None", editState == EditState::None)) { editState = EditState::None; }
			ImGui::SameLine();
			if (ImGui::RadioButton("Enemy", editState == EditState::Enemy)) { editState = EditState::Enemy; }
			ImGui::SameLine();
			if (ImGui::RadioButton("Meteorite", editState == EditState::Meteorite)) { editState = EditState::Meteorite; }
		}
	}

	// DemoPlay項目
	if (ImGui::CollapsingHeader("DemoPlay", ImGuiTreeNodeFlags_DefaultOpen)) {
		// 非デモプレイ中の処理
		if (!isPlayDemo) {
			// スタートボタン
			if (ImGui::Button("StartDemoPlay") && 
				// どっちもFalseの場合は反応しないようにする
				(isPlayDemoEnemy || isPlayDemoMeteorite)) {
				if (isPlayDemoEnemy) {
					enemyEditor->run_demo_play();
				}
				if (isPlayDemoMeteorite) {
					meteoriteEditor->run_demo_play();
				}
				isPlayDemo = true;
			}
			// 各種GUIを描画するか
			ImGui::Checkbox("DemoPlayEnemy", &isPlayDemoEnemy);
			ImGui::SameLine();
			ImGui::Checkbox("DemoPlayMeteorite", &isPlayDemoMeteorite);
		}
		// デモプレイ中の処理
		else {
			// 止めるボタン
			if (ImGui::Button("StopDemoPlay")) {
				enemyEditor->stop_demo_play();
				meteoriteEditor->stop_demo_play();
				isPlayDemo = false;
			}
		}
	}
	ImGui::End();
}

#endif // _DEBUG
