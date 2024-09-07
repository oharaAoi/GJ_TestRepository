#include "EnemyManager.h"

EnemyManager::EnemyManager() { Init(); }
EnemyManager::~EnemyManager() {}

void EnemyManager::Init() {

}

void EnemyManager::Update() {
	for (std::unique_ptr<Enemy>& enemy : enemyList_) {
		enemy->Update();
	}
}

#ifdef _DEBUG

void EnemyManager::Begin_Rendering(const Camera3D& camera3D) {
	for (std::unique_ptr<Enemy>& enemy : enemyList_) {
		enemy->begin_rendering(camera3D);
	}
}

void EnemyManager::Draw() const {
	for (const std::unique_ptr<Enemy>& enemy : enemyList_) {
		enemy->draw();
	}
}
#include <externals/imgui/imgui.h>
void EnemyManager::EditImGui() {
	ImGui::Begin("EnemyManager");
	CreateConfigGui();
	ImGui::End();
}

void EnemyManager::CreateConfigGui() {
	if (ImGui::TreeNode("CreateConfig")) {

		// 生成ボタン
		if (ImGui::Button("create")) {

		}
		ImGui::SameLine();
		// テキストの入力
		if (ImGui::InputText("File Name", fileNameBuffer_, IM_ARRAYSIZE(fileNameBuffer_))) {
			createFileName_ = fileNameBuffer_;
		}

		ImGui::DragFloat3("position", &createPos_.x, 0.1f);
		if (ImGui::RadioButton("Type 1", createType_ == EnemyType::Normal_Type)) {
			createType_ = EnemyType::Normal_Type;
		}
		if (ImGui::RadioButton("Type 2", createType_ == EnemyType::SpecialPop_Type)) {
			createType_ = EnemyType::SpecialPop_Type;
		}

		if (ImGui::Button("pop")) {
			enemyList_.emplace_back(std::make_unique<Enemy>(createPos_, createType_));
		}

		ImGui::TreePop();
	}
}

void EnemyManager::EditConfigGui() {
	if (ImGui::TreeNode("EditConfig")) {

		ImGui::TreePop();
	}
}

void EnemyManager::SaveFile() {
	json root;
	// rootにデータを書き込む
	uint32_t index = 0;
	for (SettingData& data : settingData_) {
		std::string enemyId = "enemy" + std::to_string(index);

		json pos = json::array({ data.position.x,data.position.y,data.position.z });
		root[createFileName_][enemyId] = {
			{"position", pos},
			{"enemyType", data.enemyType}
		};

		++index;
	}

	std::string filePath = kDirectoryPath_ + createFileName_ + ".json";
	std::ofstream ofs;
	// ファイルを書き込みように開く
	ofs.open(filePath);

	// ファイルオープンが出来ているか
	if (ofs.fail()) {
		std::string message = "Faild open data file for write";
		assert(0);
		return;
	}

	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl; // rootにあるデータをjson文字列に変換してファイルへ
	// 閉じる
	ofs.close();
}

#endif // _DEBUG

void EnemyManager::LoadFile() {
	// 読み込むjsonファイルのフルパスを合成する
	std::string filePath = kDirectoryPath_ + currentFileName_ + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込みように開く
	ifs.open(filePath);

	if (ifs.fail()) {
		std::string message = "not Exist " + currentFileName_ + ".json";
		return;
	}

	json root;
	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	json::iterator itGroup = root.find(currentFileName_);
	assert(itGroup != root.end());
	for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
		const std::string& itemName = itItem.key();
		if (itItem->is_number_integer()) {
			// int型の値を取得
			uint32_t value = itItem->get<uint32_t>();
			SetValue(currentFileName_, itemName, value);
		} else if (itItem->is_array() && itItem->size() == 3) {
			// float型のjson配列登録
			Vector3 value = { itItem->at(0), itItem->at(1), itItem->at(2) };
			SetValue(currentFileName_, itemName, value);
		} 
	}
}
