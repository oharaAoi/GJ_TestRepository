#include "MeteoriteManager.h"
#include "Game/Scene/GameScene.h"

MeteoriteManager::MeteoriteManager() {}
MeteoriteManager::~MeteoriteManager() {}

void MeteoriteManager::Init() {
	LoadAllFile();

	SelectionArrange();
}

void MeteoriteManager::Update(const Vector3& playerPosition) {
	// 選出されているファイルの選出不可時間を更新する
	auto it = groupRefs_.begin();
	while (it != groupRefs_.end()) {
		if ((*it)->spawnedFrameCount == 0) {
			it = groupRefs_.erase(it);
		} else {
			(*it)->spawnedFrameCount--;
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


	for (auto& times : timedCalls_) {
		times.Update();
	}

	timedCalls_.remove_if([](const TimedCall& timeCalls) {
		if (timeCalls.IsFinished()) {
			return true;
		}
		return false;
	});
}

// ------------------- ファイルの選出を行う ------------------- //
void MeteoriteManager::SelectionArrange() {

	// 抽選を行う
	std::string randomKey = GetRandomKey();
	/*float popWidth = GetValue<float>(randomKey, "rePopWidth");
	float randomZ = RandomFloat(-popWidth, popWidth);*/

	// 選出されたファイルに格納されているデータの名前を取得する
	std::vector<std::string> itemArray;
	for (const auto& pair : groupMap_[randomKey].items) {
		const std::string& category = pair.first;
		itemArray.push_back(category);
	}

	// 隕石の位置を取得する
	for (uint32_t oi = 0; oi < itemArray.size(); ++oi) {
		if (itemArray[oi] != "Adjustment") {
			gameScene_->AddMeteorite(GetValue<Vector3>(randomKey, itemArray[oi]));
			//gameScene_->AddMeteorite(Vector3{RandomFloat(14, 18), 0, RandomFloat(-8, 8) });
		}
	}

	// 一定時間選出出来ないようにする
	groupMap_[randomKey].spawnedFrameCount = GetValue<Adjustment>(randomKey, "Adjustment").rePopTime;

	// 選出出来ないグループを配列にまとめる
	groupRefs_.push_back(&groupMap_[randomKey]);

	// timedCallをリセットする
	timedCalls_.push_back(TimedCall(std::bind(&MeteoriteManager::SelectionArrange, this), kAppearanceTime_));
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
		if (groupMap_[randomKey].spawnedFrameCount == 0) {
			return randomKey;
		}

		loopCount++;
	}
}

#ifdef _DEBUG
#include "externals/imgui/imgui.h"
void MeteoriteManager::EditImGui() {
	ImGui::Begin("(MeteoriteManger)");

	if (ImGui::Button("ReLoad")) {
		LoadAllFile();
	}

	CreateConfigGui();

	EditConfigGui();
	
	ImGui::End();
}

void MeteoriteManager::CreateConfigGui() {
	if (ImGui::TreeNode("CreateConfig")) {
		ImGui::DragScalar("rePopTime", ImGuiDataType_U32, &rePopTime_);
		ImGui::DragFloat("popWidth", &popWidth_);
		ImGui::DragScalar("popNum", ImGuiDataType_U32, &popNum_);
		ImGui::DragFloat3("editPos", &editMeteoPos_.x, 0.01f);
		if (ImGui::Button("pop")) {
			debugMeteoriteList_.emplace_back(editMeteoPos_);
			popedList_.emplace_back(editMeteoPos_);
		}

		if (ImGui::TreeNode("popedList")) {
			uint32_t popIndex = 0;
			for (std::list<Vector3>::iterator popedPos = popedList_.begin(); popedPos != popedList_.end();) {
				Vector3 pos = *popedPos;
				std::string name = "popedPos" + std::to_string(popIndex);
				ImGui::DragFloat3(name.c_str(), &pos.x, 0.01f);
				ImGui::SameLine();
				std::string popButton = "RePop" + std::to_string(popIndex);
				if (ImGui::Button(popButton.c_str())) {
					debugMeteoriteList_.emplace_back(pos);
				}
				ImGui::SameLine();

				std::string deleteButton = "Delete" + std::to_string(popIndex);
				if (ImGui::Button(deleteButton.c_str())) {
					popedPos = popedList_.erase(popedPos);
					ImGui::TreePop();
					ImGui::End();
					return;
				}

				++popedPos;
				++popIndex;
			}
			ImGui::TreePop();
		}

		// saveを行う
		if (ImGui::Button("Save")) {
			std::string meteoPosNumber = saveFileName_;
			// 位置の設定
			uint32_t popIndex = 0;
			for (std::list<Vector3>::iterator popedPos = popedList_.begin(); popedPos != popedList_.end();) {
				Vector3 pos = *popedPos;
				std::string name = "popPos" + std::to_string(popIndex);
				AddItem(meteoPosNumber, name, pos);
				popedPos++;
				popIndex++;
			}
			// 調整項目の設定
			Adjustment adjustment = {
				rePopTime_,
				popWidth_,
				popNum_ = popIndex,
			};
			AddItem(meteoPosNumber, "Adjustment", adjustment);

			SaveFile(meteoPosNumber);
		}

		ImGui::SameLine();
		if (ImGui::InputText("name", saveNameBuffe_, IM_ARRAYSIZE(saveNameBuffe_))) {
			saveFileName_ = saveNameBuffe_;
		}
		ImGui::TreePop();
	}

}
void MeteoriteManager::EditConfigGui() {
	if (ImGui::TreeNode("EditConfig")) {
		if (ImGui::BeginCombo("fileList", currentFile_.c_str())) {
			for (uint32_t oi = 0; oi < fileNameArray_.size(); oi++) {
				bool isSelected = (currentFile_ == fileNameArray_[oi]);
				if (ImGui::Selectable(fileNameArray_[oi].c_str(), isSelected)) {
					currentFile_ = fileNameArray_[oi];
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
#endif 

// ------------------- directory内にあるパスを取得 ------------------- //
void MeteoriteManager::LoadAllFile() {
	for (const auto& entry : std::filesystem::directory_iterator(kDirectoryPath_)) {
		LoadFile(entry.path().stem().string());
		fileNameArray_.push_back(entry.path().filename().string());
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
				{ "rePopTime", adj.rePopTime },
				{ "rePopWidth", adj.rePopWidth },
				{ "popNum", adj.popNum }
			};

			// int32_t型の値を保持していれば
		}else if (std::holds_alternative<uint32_t>(item.value)) {
			// int32_t型の値を登録
			root[groupName][itemName] = std::get<uint32_t>(item.value);

			// float型
		} else if (std::holds_alternative<float>(item.value)) {
			// float型の値を登録
			root[groupName][itemName] = std::get<float>(item.value);

			// Vector2型
		} else if (std::holds_alternative<Vector2>(item.value)) {
			// Vector3型の値を登録
			Vector2 value = std::get<Vector2>(item.value);
			root[groupName][itemName] = json::array({ value.x, value.y });

		}else if (std::holds_alternative<Vector3>(item.value)){
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
void MeteoriteManager::LoadFile(const std::string& groupName) {
	// 読み込むjsonファイルのフルパスを合成する
	std::string filePath = kDirectoryPath_ + groupName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込みように開く
	ifs.open(filePath);

	if (ifs.fail()) {
		std::string message = "not Exist " + groupName + ".json";
		return;
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
	for (json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
		const std::string& itemName = itItem.key();
		// アイテム名を取得

		// int32_t型の値を保持していれば
		if (itItem->is_number_integer()) {
			// int型の値を取得
			uint32_t value = itItem->get<uint32_t>();
			SetValue(groupName, itemName, value);

			// float型
		} else if (itItem->is_number_float()) {
			// float型の値を取得
			double value = itItem->get<double>();
			SetValue(groupName, itemName, static_cast<float>(value));

			// Vector2
		} else if (itItem->is_array() && itItem->size() == 2) {
			// float型のjson配列登録
			Vector2 value = { itItem->at(0), itItem->at(1) };
			SetValue(groupName, itemName, value);

		} else if(itItem->is_array() && itItem->size() == 3){
			// float型のjson配列登録
			Vector3 value = { itItem->at(0), itItem->at(1), itItem->at(2) };
			SetValue(groupName, itemName, value);
			
		} else if (itItem->is_object() && itItem->contains("rePopTime") && itItem->contains("rePopWidth") && itItem->contains("popNum")) {
			Adjustment value = {
			   itItem->at("rePopTime").get<uint32_t>(),
			   itItem->at("rePopWidth").get<float>(),
			   itItem->at("popNum").get<uint32_t>()
			};
			SetValue(groupName, itemName, value);
		} 
	}
}
