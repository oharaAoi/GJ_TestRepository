#include "Game/Effect/Emitter.h"
#include "Game/Effect/EffectManager.h"
#include "externals/imgui/imgui.h"

Emitter::Emitter(EffectManager* effectManager) {
	Init(effectManager);
}

Emitter::~Emitter() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Emitter::Init(EffectManager* effectManager) {
	effectManager_ = effectManager;

	centerPos_ = { 0,0,0 };
	direction_ = { 0,1,0 };
	range_ = { 1,1,1 };
	particleScale_ = { 1,1,1 };

	lifeTime_ = 120;
	createTime_ = 120;
	frameCreateCount_ = 120;
	createCount_ = 3;

	speed_ = 1.0f;
	radius_ = 1.0f;
	angle_ = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Emitter::Update() {
	EditImGui();

	if (--frameCreateCount_ <= 0) {
		Emit();
		frameCreateCount_ = createTime_;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Emitter::Draw() const {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- particleを発射する ------------------- //
void Emitter::Emit() {
	for (uint32_t oi = 0; oi < createCount_; ++oi) {
		particleTranslation_.x = RandomFloat(-range_.x, range_.x) + centerPos_.x;
		particleTranslation_.y = RandomFloat(-range_.y, range_.y) + centerPos_.y;
		particleTranslation_.z = RandomFloat(-range_.z, range_.z) + centerPos_.z;

		particleScale_ = { radius_, radius_, radius_ };

		// 向きに対しての角度を算出
		float minAngle = -angle_ * ToRadian;
		float maxAngle = angle_ * ToRadian;
		float randomAngle = RandomFloat(minAngle, maxAngle);

		particleVelocity_ = direction_;
		particleVelocity_.normalize();

		effectManager_->AddParticleList(particleTranslation_, particleScale_, particleVelocity_, lifeTime_, speed_);
	}
}

// ------------------- ImGuiを編集する ------------------- //
void Emitter::EditImGui() {
#ifdef _DEBUG
	if (ImGui::TreeNode("CreateEmitter")) {
		ImGui::DragScalar("frameCreateCount", ImGuiDataType_U32, &frameCreateCount_);
		ImGui::DragFloat3("centerPos", &centerPos_.x, 0.01f);
		ImGui::DragFloat3("direction", &direction_.x, 0.01f);
		ImGui::DragFloat3("range", &range_.x, 0.01f);
		ImGui::DragFloat("speed", &speed_, 0.1f);
		ImGui::DragFloat("angle", &angle_, 1.0f, -360.0f, 360.0f);
		ImGui::DragScalar("createTime", ImGuiDataType_U32, &createTime_);
		ImGui::DragScalar("createCount", ImGuiDataType_U32, &createCount_);
		ImGui::DragScalar("lifeTime", ImGuiDataType_U32, &lifeTime_);
		PolygonMeshManager::MeshListGui(useObjName_);

		// 保存したいEmitterの名前を登録
		char buffer[128];
		// std::stringからバッファにコピー
		strncpy_s(buffer, emitterName_.c_str(), sizeof(buffer));
		// ImGuiで文字を入力
		if (ImGui::InputText("Label", buffer, IM_ARRAYSIZE(buffer))) {
			// バッファからstd::stringにコピー
			emitterName_ = buffer;
		}

		// 現在のEmitterの情報を保存する
		if (ImGui::Button("Save")) {
			CreateGroup(emitterName_);
			AddItem(emitterName_, "centerPos", centerPos_);
			AddItem(emitterName_, "direction", direction_);
			AddItem(emitterName_, "range", range_);
			AddItem(emitterName_, "createTime", createTime_);
			AddItem(emitterName_, "frameCreateCount", frameCreateCount_);
			AddItem(emitterName_, "createCount", createCount_);
			AddItem(emitterName_, "lifeTime", lifeTime_);
			AddItem(emitterName_, "speed", speed_);
			AddItem(emitterName_, "radius", radius_);
			AddItem(emitterName_, "angle", angle_);
			/*AddItem(emitterName_, "useObjectName", useObjName_);
			AddItem(emitterName_, "emitterName", emitterData_);*/
		}
		
		ImGui::TreePop();
	}

#endif // _DEBUG
}

// ------------------- Emitterのグループを作成する ------------------- //
void Emitter::CreateGroup(const std::string& groupName) {
	emitterData_[groupName];
}

// ------------------- Emitterの情報をファイルに格納する ------------------- //
void Emitter::SaveFileEmitter(const std::string& groupName) {
	// グループ検索
	std::map<std::string, Group>::iterator itGroup = emitterData_.find(groupName);
	// 未登録チェック
	assert(itGroup != emitterData_.end());

	json root;
	root = json::object();
	// jsonオブジェクト登録
	root[groupName] = json::object();

	// ------------------------------------------
	// ↓ jsonに書き込む用の変数にデータを入れる
	// ------------------------------------------
	for (std::map<std::string, Item>::iterator itItem = itGroup->second.items.begin(); itItem != itGroup->second.items.end(); ++itItem) {
		// 項目名を取得
		const std::string& itemName = itItem->first;
		// 項目の参照を取得
		Item& item = itItem->second;

		// int32_t型の値を保持していれば
		if (std::holds_alternative<uint32_t>(item.value)) {
			// int32_t型の値を登録
			root[groupName][itemName] = std::get<uint32_t>(item.value);

			// float型
		} else if (std::holds_alternative<float>(item.value)) {
			// float型の値を登録
			root[groupName][itemName] = std::get<float>(item.value);

			// Vector3型
		} else if (std::holds_alternative<Vector3>(item.value)) {
			// Vector3型の値を登録
			Vector3 value = std::get<Vector3>(item.value);
			root[groupName][itemName] = json::array({ value.x, value.y, value.z });

			// bool
		} else if (std::holds_alternative<bool>(item.value)) {
			root[groupName][itemName] = std::get<bool>(item.value);
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

// ------------------- Emitterの情報を読み込む ------------------- //
void Emitter::LoadEmitter(const std::string& groupName) {
	// 読み込むjsonファイルのフルパスを合成する
	std::string filePath = kDirectoryPath_ + groupName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込みように開く
	ifs.open(filePath);

	if (ifs.fail()) {
		std::string message = "not Exist " + groupName + ".json";
		assert(0);
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

	// 各項目についての処理
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

			// Vector3
		} else if (itItem->is_array() && itItem->size() == 3) {
			// float型のjson配列登録
			Vector3 value = { itItem->at(0), itItem->at(1), itItem->at(2) };
			SetValue(groupName, itemName, value);

			// bool
		} else if (itItem->is_boolean()) {
			bool flag = itItem->get<bool>();
			SetValue(groupName, itemName, flag);
		} 
	}
}
