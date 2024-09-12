#include "MeteoriteManager.h"
#include "Game/Scene/GameScene.h"

#include "Game/Editor/EditorController.h"

MeteoriteManager::MeteoriteManager(std::list<std::unique_ptr<Meteorite>>& sceneMeteoList, CollisionManager* collisionManager)
	: sceneMeteoList_(sceneMeteoList), collisionManager_(collisionManager) {
	Init();
}
MeteoriteManager::~MeteoriteManager() {}

void MeteoriteManager::Init() {
	LoadAllFile();
	isStart_ = false;
	//SelectionArrange();
}

void MeteoriteManager::Update(const Vector3& playerPosition) {
	// 選出されているファイルの選出不可時間を更新する
	auto it = groupRefs_.begin();
	while (it != groupRefs_.end()) {
		if ((*it)->spawnedFrameCount <= 0) {
			it = groupRefs_.erase(it);
		}
		else {
			(*it)->spawnedFrameCount -= GameTimer::DeltaTime();
			++it;
		}
	}

	for (Meteorite& meteo : meteoriteList_) {
		meteo.Update(playerPosition);
	}

	// 死亡フラグのチェックを行う
	meteoriteList_.remove_if([](const Meteorite& meteo) {
		if (meteo.GetIsDead()) {
			return true;
		}
		return false;
	});

#ifdef _DEBUG
	if (editController && editController->is_edit()) {
		return;
	}
#endif // _DEBUG

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

void MeteoriteManager::StartPop() {
	if (!isStart_) {
		SelectionArrange();
		isStart_ = false;
	}
}

// ------------------- ファイルの選出を行う ------------------- //
void MeteoriteManager::SelectionArrange() {

	// 抽選を行う
	std::string randomKey = GetRandomKey();
	/*float popWidth = GetValue<float>(randomKey, "rePopWidth");
	float randomZ = RandomFloat(-popWidth, popWidth);*/

	// 未登録チェック
	assert(groupMap_.contains(randomKey));

	// 選出されたファイルに格納されているデータの名前を取得する
	PopFromGroup(groupMap_[randomKey]);

	// 一定時間選出出来ないようにする
	groupMap_[randomKey].spawnedFrameCount = GetValue<Adjustment>(groupMap_[randomKey], "Adjustment").repopTime;

	// 選出出来ないグループを配列にまとめる
	groupRefs_.push_back(&groupMap_[randomKey]);

	// timedCallをリセットする
	timedCalls_.push_back(Test::TimedCall(std::bind(&MeteoriteManager::SelectionArrange, this), kAppearanceTime_));
}

void MeteoriteManager::PopFromGroup(const MeteoriteManager::Group& group) {
	for (const auto& item : group.items) {
		if (item.second.value.index() == 3) {
			AddMeteo(GetValue<Vector3>(group, item.first));
		}
	}
}

// ------------------- ランダムなファイルを選出する ------------------- //
std::string MeteoriteManager::GetRandomKey() {
	// マップのキーを配列に格納
	uint32_t loopCount = 0;
	while (true) {
		if (loopCount > 10) {
			return groupMap_.begin()->first;
		}

		// mapに格納されているキーを配列に移す
		std::vector<std::string> keys;
		for (const auto& pair : groupMap_) {
			keys.push_back(pair.first);
		}

		// 乱数で番号を出し、対応するファイル名を取り出す
		int randomKeyIndex = RandomInt(0, static_cast<int>(keys.size()) - 1);
		std::string randomKey = keys[randomKeyIndex];

		// 選ばれたキーを使用しマップを参照して使用可能かを確認
		if (groupMap_[randomKey].spawnedFrameCount <= 0) {
			return randomKey;
		}

		loopCount++;
	}
}

#ifdef _DEBUG
void MeteoriteManager::DebugGui() {
	for (auto& times : timedCalls_) {
		times.DebugGui();
	}
}
#endif // _DEBUG

#ifdef _DEBUG

void MeteoriteManager::SetEditor(const EditorController* editController_) {
	editController = editController_;
}
#endif 

void MeteoriteManager::AddMeteo(const Vector3& position) {
	auto& newMeteo = sceneMeteoList_.emplace_back(std::make_unique<Meteorite>(position));
	collisionManager_->register_collider("Meteo", newMeteo->GetCollider());
}

// ------------------- directory内にあるパスを取得 ------------------- //
void MeteoriteManager::LoadAllFile() {
	for (const auto& entry : std::filesystem::directory_iterator(kDirectoryPath_)) {
		std::string fileName = entry.path().stem().string();
		auto result = LoadFile(fileName);
		if (result.has_value()) {
			groupMap_[fileName] = std::move(result.value());
		}
	}
}

// ------------------- ファイルを保存する ------------------- //
void MeteoriteManager::SaveFile(const std::string& groupName) {
	// グループ検索
	std::map<std::string, Group>::iterator itGroup = groupMap_.find(groupName);
	// 未登録チェック
	assert(itGroup != groupMap_.end());

	json root;
	root = json::object();
	// jsonオブジェクト登録
	root[groupName] = json::object();

	for (std::map<std::string, Items>::iterator itItem = itGroup->second.items.begin(); itItem != itGroup->second.items.end(); ++itItem) {
		// 項目名を取得
		const std::string& itemName = itItem->first;
		// 項目の参照を取得
		Items& item = itItem->second;

		if (std::holds_alternative<Adjustment>(item.value)) {
			Adjustment adj = std::get<Adjustment>(item.value);
			// Adjustment型の値を登録
			root[groupName]["Adjustment"] = {
				{ "rePopTime", adj.repopTime },
				{ "rePopWidth", adj.popWidth },
				{ "popNum", adj.numMeteorites }
			};

			// int32_t型の値を保持していれば
		}
		else if (std::holds_alternative<uint32_t>(item.value)) {
			// int32_t型の値を登録
			root[groupName][itemName] = std::get<uint32_t>(item.value);

			// float型
		}
		else if (std::holds_alternative<float>(item.value)) {
			// float型の値を登録
			root[groupName][itemName] = std::get<float>(item.value);

			// Vector2型
		}
		else if (std::holds_alternative<Vector2>(item.value)) {
			// Vector3型の値を登録
			Vector2 value = std::get<Vector2>(item.value);
			root[groupName][itemName] = json::array({ value.x, value.y });

		}
		else if (std::holds_alternative<Vector3>(item.value)) {
			// Vector3型の値を登録
			Vector3 value = std::get<Vector3>(item.value);
			root[groupName][itemName] = json::array({ value.x, value.y, value.z });

		}
	}

	// ------------------------------------------
	// ↓ ディレクトリがなければ作成する
	// ------------------------------------------
	std::filesystem::path dire(kDirectoryPath_);
	if (!std::filesystem::exists(kDirectoryPath_)) {
		std::filesystem::create_directories(kDirectoryPath_);
	}

	// ------------------------------------------
	// ↓ 書き込みを行う
	// ------------------------------------------
	// 書き込むJSONファイルのフルパス
	std::string filePath = kDirectoryPath_ + groupName + ".json";
	// 書き込み用のファイルストリーム
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

// ------------------- ファイルを読み込む ------------------- //
std::optional<MeteoriteManager::Group> MeteoriteManager::LoadFile(const std::string& groupName) {
	// 読み込むjsonファイルのフルパスを合成する
	std::string filePath = kDirectoryPath_ + groupName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込みように開く
	ifs.open(filePath);

	if (ifs.fail()) {
		std::string message = "not Exist " + groupName + ".json";
		return std::nullopt;
	}

	json root;
	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();
	// グループがあるかを検索
	json::iterator itGroup = root.find(groupName);
	// 未登録チェック

	assert(itGroup != root.end());

	Group newGroup{};

	for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
		const std::string& itemName = itItem.key();
		// アイテム名を取得

		// int32_t型の値を保持していれば
		if (itItem->is_number_integer()) {
			// int型の値を取得
			uint32_t value = itItem->get<uint32_t>();
			SetValue(newGroup, itemName, value);

			// float型
		}
		else if (itItem->is_number_float()) {
			// float型の値を取得
			double value = itItem->get<double>();
			SetValue(newGroup, itemName, static_cast<float>(value));

			// Vector2
		}
		else if (itItem->is_array() && itItem->size() == 2) {
			// float型のjson配列登録
			Vector2 value = { itItem->at(0), itItem->at(1) };
			SetValue(newGroup, itemName, value);

		}
		else if (itItem->is_array() && itItem->size() == 3) {
			// float型のjson配列登録
			Vector3 value = { itItem->at(0), itItem->at(1), itItem->at(2) };
			SetValue(newGroup, itemName, value);

		}
		else if (itItem->is_object() && itItem->contains("NumMeteorites") && itItem->contains("RepopTime") && itItem->contains("PopWidth")) {
			Adjustment value = {
			   itItem->at("RepopTime").get<float>(),
			   itItem->at("PopWidth").get<float>(),
			   itItem->at("NumMeteorites").get<uint32_t>()
			};
			SetValue(newGroup, itemName, value);
		}
	}
	return newGroup;
}
