#ifdef _DEBUG
#include "EnemyPopEditor.h"

#include "externals/imgui/imgui.h"
#include "externals/nlohmann/json.hpp"

void EnemyPopEditor::initialize(EnemyManager* manager_) {
	manager = manager_;
	get_group_map_keys();

	inportFileName.reserve(64);
	resultString.reserve(1000);
}

void EnemyPopEditor::draw_editor() {
	edit_window();
	group_config();
}

void EnemyPopEditor::debug_draw3d(const Camera3D* camera) {
	for (auto& obj : editPopData) {
		obj.object.begin_rendering(*camera);
		obj.object.draw();
	}
}

void EnemyPopEditor::run_demo_play() {
	compress();
	manager->PopFromGroup(editGroup);
}

void EnemyPopEditor::stop_demo_play() {
}

void EnemyPopEditor::edit_window() {
	ImGui::Begin("EnemyEditorMain");
	// エクスポート処理
	if (ImGui::Button("Export")) {
		resultString = save(exportFileName);
		manager->LoadFileName();
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
		manager->LoadFileName();
		get_group_map_keys();
	}
	ImGui::End();
}

void EnemyPopEditor::group_config() {
	ImGui::Begin("GroupConfig##Enemy");

	if (ImGui::Button("AddMeteorite")) {
		auto& newData = editPopData.emplace_back();
		newData.object.reset_object("kariEnemy.obj");
	}

	if (ImGui::CollapsingHeader("MeteoriteList", ImGuiTreeNodeFlags_DefaultOpen)) {
		uint32_t popIndex = 0;
		for (auto&& itr = editPopData.begin(); itr != editPopData.end();) {
			auto& data = itr->data;
			std::string name = "##Meteorite" + std::to_string(popIndex);
			ImGui::PushItemWidth(200);
			ImGui::DragFloat3(name.c_str(), &data.position.x, 0.01f);
			data.position.y = 13.0f;
			itr->object.get_transform().set_translate(data.position);
			if (ImGui::RadioButton("Normal", data.enemyType == EnemyType::Normal_Type)) { data.enemyType = EnemyType::Normal_Type;}
			ImGui::SameLine();
			if (ImGui::RadioButton("Special", data.enemyType == EnemyType::SpecialPop_Type)) { data.enemyType = EnemyType::SpecialPop_Type; }
			ImGui::SameLine();
			std::string deleteButton = "Delete##" + std::to_string(popIndex);
			if (ImGui::Button(deleteButton.c_str())) {
				itr = editPopData.erase(itr);
			}
			else {
				++popIndex;
				++itr;
			}
		}
	}
	ImGui::End();
}

std::string EnemyPopEditor::save(const std::string& fileName) {
	compress();
	if (fileName.empty()) {
		return "FileName is void.";
	}
	if (editPopData.empty()) {
		return "Pop enemy group is empty.";
	}
	json root;
	// rootにデータを書き込む
	uint32_t index = 0;
	for (const PopData& popData : editPopData) {
		std::string enemyId = "enemy" + std::to_string(index);
		auto& data = popData.data;
		json pos = json::array({ data.position.x,data.position.y,data.position.z });
		root[fileName][enemyId] = {
			{"position", pos},
			{"enemyType", data.enemyType}
		};

		++index;
	}

	// ------------------------------------------
	// ↓ ディレクトリがなければ作成する
	// ------------------------------------------
	std::filesystem::path dire(manager->kDirectoryPath_);
	if (!std::filesystem::exists(manager->kDirectoryPath_)) {
		std::filesystem::create_directories(manager->kDirectoryPath_);
	}

	std::string filePath = manager->kDirectoryPath_ + fileName + ".json";
	std::ofstream ofs;
	// ファイルを書き込みように開く
	ofs.open(filePath);

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

void EnemyPopEditor::compress() {
	editGroup.items.clear();
	for (int i = 0; i < editPopData.size(); ++i) {
		std::string enemyId = "Enemy" + std::to_string(i);

		editGroup.items[enemyId] = editPopData[i].data;
	}
}

void EnemyPopEditor::decompress() {
	editPopData.clear();
	for (auto& items : editGroup.items | std::views::values) {
		editPopData.emplace_back(
			EnemyManager::SettingData{ items.position, items.enemyType }, 
			GameObject("kariEnemy.obj")
		);
	}
}

void EnemyPopEditor::get_group_map_keys() {
	fileList.clear();
	for (const auto& group : manager->loadData_ | std::views::keys) {
		fileList.emplace_back(group);
	}
}
#endif // _DEBUG
