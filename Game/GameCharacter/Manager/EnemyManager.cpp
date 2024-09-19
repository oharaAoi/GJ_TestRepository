#include "EnemyManager.h"

EnemyManager::EnemyManager(std::list<std::unique_ptr<Enemy>>& sceneEnemyList,
						   CollisionManager* collisionManager, bool* isPlayerFlagPtr, const Hierarchy& fieldHierarchy)
	: sceneEnemyList_(sceneEnemyList), collisionManager_(collisionManager), isPlayerFlagPtr_(isPlayerFlagPtr),
	fieldHierarchy_(fieldHierarchy){
	Init();
}
EnemyManager::~EnemyManager() {}

void EnemyManager::Init() {
	LoadFileName();

	//SelectArrange();
	//AddEnemy(Vector3{ 2.0f, 6.0f, 0.0f }, EnemyType::Normal_Type);
}

void EnemyManager::Update(const Vector3& playerPosition) {
	/*for (std::unique_ptr<Enemy>& enemy : enemyList_) {
		enemy->Update(playerPosition);
	}*/

	// -------------------------------------------------
	// ↓ 敵の上限設定
	// -------------------------------------------------
	if (CheckConstrainToField() >= 4) {
		return;
	}

	if (sceneEnemyList_.size() >= 8) {
		return;
	}

	// -------------------------------------------------
	// ↓ TImedCallの設定
	// -------------------------------------------------
	for (auto& times : timedCalls_) {
		times.update();
	}

	timedCalls_.remove_if([](const TimedCall<void(void)>& timeCalls) {
		if (timeCalls.is_finished()) {
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
			fieldOnEnemyNum++;
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

// ------------------- ファイルの選出を行う ------------------- //
void EnemyManager::SelectArrange() {
	std::vector<std::string> keyArray;
	for (const auto& pair : loadData_) {
		keyArray.push_back(pair.first);
	}

	int randomNum = RandomInt(0, static_cast<int>(keyArray.size() - 1));
	PopFromGroup(loadData_[keyArray[randomNum]]);

	// timedCallをリセットする
	timedCalls_.push_back(TimedCall<void(void)>(std::bind(&EnemyManager::SelectArrange, this), popTime_));
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
	ImGui::Text("popTime:%f", popTime_);

	if (ImGui::Button("popCenter")) {
		AddEnemy(Vector3{ 0.0f, 6.0f, 0.0f }, EnemyType::Normal_Type);
	}

	/*CreateConfigGui();
	EditConfigGui();*/
	ImGui::End();
}

#endif // _DEBUG

void EnemyManager::AddEnemy(const Vector3& positoin, const EnemyType& type) {
	auto& newEnemy = sceneEnemyList_.emplace_back(std::make_unique<Enemy>(positoin, type));
	collisionManager_->register_collider("Enemy", newEnemy->GetCollider());
	newEnemy->SetIsPlayerFlragPtr(isPlayerFlagPtr_);
	newEnemy->set_parent(fieldHierarchy_);
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
