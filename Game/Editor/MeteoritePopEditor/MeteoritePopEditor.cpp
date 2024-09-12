#ifdef _DEBUG
#include "MeteoritePopEditor.h"

#include "externals/imgui/imgui.h"
#include "externals/nlohmann/json.hpp"

#include <format>

void MeteoritePopEditor::initialize(MeteoriteManager* manager_) {
	manager = manager_;
	get_group_map_keys();

	inportFileName.reserve(64);
	resultString.reserve(1000);
}

void MeteoritePopEditor::draw_editor() {
	edit_window();
	group_config();
}

void MeteoritePopEditor::edit_window() {
	ImGui::Begin("MeteoriteEditorMain");
	// エクスポート処理
	if (ImGui::Button("Export")) {
		resultString = save(exportFileName);
		manager->LoadAllFile();
		get_group_map_keys();
	}
	ImGui::SameLine();
	ImGui::PushItemWidth(150);
	ImGui::InputText(".json##ExportFileName", &exportFileName[0], sizeof(char) * 64);
	ImGui::Text(resultString.c_str());

	//インポート処理
	if (ImGui::Button("Inport")) {
		auto loaded = manager->LoadFile(inportFileName.c_str());
		if (loaded.has_value()) {
			editGroup = std::move(loaded.value());
			decompress();
		}
	}
	ImGui::SameLine();
	static int index = 0;
	if (!fileList.empty()) {
		inportFileName = fileList[index];
	}
	ImGui::PushItemWidth(200);
	if (ImGui::BeginCombo("##InportFileName", &inportFileName[0], ImGuiComboFlags_HeightLargest)) {
		for (int i = 0; i < fileList.size(); i++) {
			const bool isSelected = (index == i);
			if (ImGui::Selectable(fileList[i].c_str(), isSelected)) {
				index = i;
			}
			if (isSelected) {
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	if (ImGui::Button("ReloadList")) {
		manager->LoadAllFile();
		get_group_map_keys();
	}
	ImGui::End();
}

void MeteoritePopEditor::group_config() {
	ImGui::Begin("GroupConfig##Meteorite");
	ImGui::DragFloat("RepopTime", &repopTime, 0.1f, 0.0f, 100000.0f, "%.2fs");
	ImGui::DragFloat("PopWidth", &popWidth, 0.01f);

	if (ImGui::Button("AddMeteorite")) {
		popList.emplace_back();
		auto& newObj = debugDrawObject.emplace_back("particle.obj");
		newObj.get_transform().set_translate(popList.back());
	}

	if (ImGui::CollapsingHeader("MeteoriteList", ImGuiTreeNodeFlags_DefaultOpen)) {
		uint32_t popIndex = 0;
		for (auto&& position = popList.begin(); position != popList.end();) {
			std::string name = "##Meteorite" + std::to_string(popIndex);
			ImGui::PushItemWidth(200);
			ImGui::DragFloat3(name.c_str(), &position->x, 0.01f);
			position->y = 14.0f;
			debugDrawObject[popIndex].get_transform().set_translate(*position);
			ImGui::SameLine();
			std::string deleteButton = "Delete##" + std::to_string(popIndex);
			if (ImGui::Button(deleteButton.c_str())) {
				position = popList.erase(position);
				debugDrawObject.erase(debugDrawObject.begin() + popIndex);
			}
			else {
				++popIndex;
				++position;
			}
		}
	}
	ImGui::End();
}

void MeteoritePopEditor::debug_draw3d(const Camera3D* camera) {
	for (auto& obj : debugDrawObject) {
		obj.begin_rendering(*camera);
		obj.draw();
	}
}

void MeteoritePopEditor::run_demo_play() {
	compress();
	manager->PopFromGroup(editGroup);
}

void MeteoritePopEditor::stop_demo_play() {
}

std::string MeteoritePopEditor::save(const std::string& fileName) {
	compress();
	if (fileName.empty()) {
		return "FileName is void.";
	}
	if (popList.empty()) {
		return "Group pop meteorite is empty.";
	}
	json root;
	root = json::object();
	// jsonオブジェクト登録
	root[fileName] = json::object();

	for (std::map<std::string, MeteoriteManager::Items>::iterator itItem = editGroup.items.begin(); itItem != editGroup.items.end(); ++itItem) {
		// 項目名を取得
		const std::string& itemName = itItem->first;
		// 項目の参照を取得
		MeteoriteManager::Items& item = itItem->second;

		if (std::holds_alternative<MeteoriteManager::Adjustment>(item.value)) {
			MeteoriteManager::Adjustment adj = std::get<MeteoriteManager::Adjustment>(item.value);
			// Adjustment型の値を登録
			root[fileName]["Adjustment"] = {
				{ "RepopTime", adj.repopTime },
				{ "PopWidth", adj.popWidth },
				{ "NumMeteorites", adj.numMeteorites }
			};

			// int32_t型の値を保持していれば
		}
		else if (std::holds_alternative<uint32_t>(item.value)) {
			// int32_t型の値を登録
			root[fileName][itemName] = std::get<uint32_t>(item.value);

			// float型
		}
		else if (std::holds_alternative<float>(item.value)) {
			// float型の値を登録
			root[fileName][itemName] = std::get<float>(item.value);

			// Vector2型
		}
		else if (std::holds_alternative<Vector2>(item.value)) {
			// Vector3型の値を登録
			Vector2 value = std::get<Vector2>(item.value);
			root[fileName][itemName] = json::array({ value.x, value.y });

		}
		else if (std::holds_alternative<Vector3>(item.value)) {
			// Vector3型の値を登録
			Vector3 value = std::get<Vector3>(item.value);
			root[fileName][itemName] = json::array({ value.x, value.y, value.z });

		}
		else {
			return std::format("Group content \'{}\' is vaild", itemName);
		}
	}

	// ------------------------------------------
	// ↓ ディレクトリがなければ作成する
	// ------------------------------------------
	std::filesystem::path dire(manager->kDirectoryPath_);
	if (!std::filesystem::exists(manager->kDirectoryPath_)) {
		std::filesystem::create_directories(manager->kDirectoryPath_);
	}

	// ------------------------------------------
	// ↓ 書き込みを行う
	// ------------------------------------------
	// 書き込むJSONファイルのフルパス
	std::filesystem::path filePath = manager->kDirectoryPath_ + fileName.c_str() + ".json";
	// 書き込み用のファイルストリーム
	std::ofstream ofs{ filePath };

	// ファイルオープンが出来ているか
	if (ofs.fail()) {
		return "Faild open file.";
	}

	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl; // rootにあるデータをjson文字列に変換してファイルへ
	// 閉じる
	ofs.close();
	return "Successed.";
}

void MeteoritePopEditor::compress() {
	editGroup.items.clear();
	manager->AddItem(editGroup, "Adjustment", MeteoriteManager::Adjustment{ repopTime, popWidth,static_cast<uint32_t>(popList.size()) });
	for (int i = 0; i < popList.size(); ++i) {
		//editGroup.items.emplace(, );
		manager->AddItem(editGroup, "Position" + std::to_string(i), popList[i]);
	}
}

void MeteoritePopEditor::decompress() {
	popList.clear();
	debugDrawObject.clear();
	MeteoriteManager::Adjustment adj = manager->GetValue<MeteoriteManager::Adjustment>(editGroup, "Adjustment");
	repopTime = adj.repopTime;
	popWidth = adj.popWidth;
	for (uint32_t i = 0; i < adj.numMeteorites; ++i) {
		popList.emplace_back(manager->GetValue<Vector3>(editGroup, "Position" + std::to_string(i)));
		auto& newObj = debugDrawObject.emplace_back("particle.obj");
	}
}
void MeteoritePopEditor::get_group_map_keys() {
	fileList.clear();
	for (const auto& group : manager->groupMap_ | std::views::keys) {
		fileList.emplace_back(group);
	}
}
#endif // _DEBUG
