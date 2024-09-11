#include "EnemyManager.h"

EnemyManager::EnemyManager(std::list<std::unique_ptr<Enemy>>& sceneEnemyList,
						   CollisionManager* collisionManager, bool* isPlayerFlagPtr)
	: sceneEnemyList_(sceneEnemyList), collisionManager_(collisionManager), isPlayerFlagPtr_(isPlayerFlagPtr) {
	Init();
}
EnemyManager::~EnemyManager() {}

void EnemyManager::Init() {
	LoadFileName();

	//SelectArrange();
	AddEnemy(Vector3{ 2.0f, 6.0f, 0.0f }, EnemyType::Normal_Type);

	fieldOnEnemyNum_ = 0;
}

void EnemyManager::Update(const Vector3& playerPosition) {
	/*for (std::unique_ptr<Enemy>& enemy : enemyList_) {
		enemy->Update(playerPosition);
	}*/

	// -------------------------------------------------
	// ↓ 敵の上限設定
	// -------------------------------------------------
	if (CheckConstrainToField() >= 5) {
		return;
	}

	// -------------------------------------------------
	// ↓ TImedCallの設定
	// -------------------------------------------------
	for (auto& times : timedCalls_) {
		times.Update();
	}

	timedCalls_.remove_if([](const Test::TimedCall& timeCalls) {
		if (timeCalls.IsFinished()) {
			return true;
		}
		return false;
						  });
}

uint32_t EnemyManager::CheckConstrainToField(){
	uint32_t fieldOnEnemyNum = 0;
	for (std::unique_ptr<Enemy>& enemy : sceneEnemyList_) {
		Vector3 translate = enemy->get_transform().get_translate();
		Vector3 distance = (translate - Vector3(0, translate.y, 0)).normalize_safe();
		// 中心からの長さ
		float lenght = Vector3::Length(translate, Vector3(0, translate.y, 0));
		if (lenght < 5.7f) {
			fieldOnEnemyNum_++;
		}
	}

	return fieldOnEnemyNum;
}

void EnemyManager::StartPop() {
	if (!isPop_) {
		SelectArrange();
		isPop_ = true;
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

// ------------------- ファイルの選出を行う ------------------- //
void EnemyManager::SelectArrange() {
	std::vector<std::string> keyArray;
	for (const auto& pair : loadData_) {
		keyArray.push_back(pair.first);
	}

	int randomNum = RandomInt(0, static_cast<int>(keyArray.size() - 1));
	PopFromGroup(loadData_[keyArray[randomNum]]);

	// timedCallをリセットする
	timedCalls_.push_back(Test::TimedCall(std::bind(&EnemyManager::SelectArrange, this), popTime_));
}


#include <externals/imgui/imgui.h>
void EnemyManager::EditImGui() {
	ImGui::Begin("EnemyManager");
	ImGui::Text("popTime:%d", popTime_);

	if (ImGui::Button("popCenter")) {
		AddEnemy(Vector3{ 0.0f, 6.0f, 0.0f }, EnemyType::Normal_Type);
	}

	/*CreateConfigGui();
	EditConfigGui();*/
	ImGui::End();
}

void EnemyManager::CreateConfigGui() {
	if (ImGui::TreeNode("CreateConfig")) {

		// 生成ボタン
		if (ImGui::Button("create")) {
			SaveFile();
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
			settingData_.emplace_back(createPos_, createType_);
		}

		ImGui::TreePop();
	}
}

void EnemyManager::EditConfigGui() {
	if (ImGui::TreeNode("EditConfig")) {
		if (ImGui::Button("inport")) {
			std::vector<std::string> itemArray;
			for (const auto& pair : loadData_[currentFileName_].items) {
				const std::string& category = pair.first;
				itemArray.push_back(category);
			}

			// 対応するデータをloadDataから持ってくる
			SettingData data;
			for (uint32_t oi = 0; oi < itemArray.size(); ++oi) {
				data.position = loadData_[currentFileName_].items[itemArray[oi]].position;
				data.enemyType = loadData_[currentFileName_].items[itemArray[oi]].enemyType;
				AddEnemy(data.position, data.enemyType);
			}

			////enemyList_.emplace_back(std::make_unique<Enemy>(data.position, data.enemyType));
		}

		ImGui::SameLine();

		if (ImGui::BeginCombo("fileList", currentFileName_.c_str())) {
			for (uint32_t oi = 0; oi < fileNameArray_.size(); oi++) {
				bool isSelected = (currentFileName_ == fileNameArray_[oi]);
				if (ImGui::Selectable(fileNameArray_[oi].c_str(), isSelected)) {
					currentFileName_ = fileNameArray_[oi];
				}
				if (isSelected) {
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
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

	// ------------------------------------------
	// ↓ ディレクトリがなければ作成する
	// ------------------------------------------
	std::filesystem::path dire(kDirectoryPath_);
	if (!std::filesystem::exists(kDirectoryPath_)) {
		std::filesystem::create_directories(kDirectoryPath_);
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

void EnemyManager::AddEnemy(const Vector3& positoin, const EnemyType& type) {
	auto& newEnemy = sceneEnemyList_.emplace_back(std::make_unique<Enemy>(positoin, type));
	collisionManager_->register_collider("Enemy", newEnemy->GetCollider());
	newEnemy->SetIsPlayerFlragPtr(isPlayerFlagPtr_);
}

void EnemyManager::LoadFileName() {
	for (const auto& entry : std::filesystem::directory_iterator(kDirectoryPath_)) {
		std::string fileName = entry.path().stem().string();
		auto result = LoadFile(fileName);
		if (result.has_value()) {
			loadData_[fileName] = result.value();
		}
		//fileNameArray_.push_back(entry.path().stem().string());
	}
}

std::optional<EnemyManager::Group> EnemyManager::LoadFile(const std::string& fileName) {
	// 読み込むjsonファイルのフルパスを合成する
	std::string filePath = kDirectoryPath_ + fileName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込みように開く
	ifs.open(filePath);

	if (ifs.fail()) {
		std::string message = "not Exist " + fileName + ".json";
		return std::nullopt;
	}

	json root;
	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	Group group;

	json::iterator itGroup = root.find(fileName);
	assert(itGroup != root.end());
	for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
		const std::string& Name = itItem.key();
		// それぞれのアイテムのデータを取得
		json itemData = itItem.value();

		// SettingData の読み込み
		Vector3 position;
		EnemyType enemyType;

		// position の読み込み (例として "position" キーがある場合)
		if (itemData.contains("position")) {
			json posData = itemData["position"];
			position.x = posData[0].get<float>();  // x 座標
			position.y = posData[1].get<float>();  // y 座標
			position.z = posData[2].get<float>();  // z 座標
		}

		// enemyType の読み込み (例として "enemyType" キーがある場合)
		if (itemData.contains("enemyType")) {
			enemyType = static_cast<EnemyType>(itemData["enemyType"].get<int>());  // int から enum への変換
		}
		group.items[Name] = SettingData(position, enemyType);
	}
	return group;
}

void EnemyManager::PopFromGroup(const Group& group) {
	for (auto& item : group.items | std::views::values) {
		AddEnemy(item.position, item.enemyType);
	}
}
