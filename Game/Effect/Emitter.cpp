#include "Game/Effect/Emitter.h"
#include "Game/Effect/EffectManager.h"
#include "externals/imgui/imgui.h"

Emitter::Emitter(EffectManager* effectManager, const std::string& emitterName,
				 const Vector3& centerPos, const Vector3& direction) {
	Init(effectManager, emitterName, centerPos, direction);
}

Emitter::~Emitter() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Emitter::Init(EffectManager* effectManager, const std::string& emitterName,
				   const Vector3& centerPos, const Vector3& direction) {
	effectManager_ = effectManager;
	emitterName_ = emitterName;

	frameCreateCount_ = 1;

	LoadEmitter(emitterName);

	centerPos_ = centerPos;
	direction_ = direction;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Emitter::Update() {
	if (emitCount_ <= 0) {
		isDead_ = true;
	}

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

		particleVelocity_ = GetEmitDirectionRandom(radius_, direction_);
		particleVelocity_.normalize();

		effectManager_->AddParticleList(particleTranslation_, particleScale_, particleVelocity_, lifeTime_, speed_);
	}

	emitCount_--;
}

// ------------------- ImGuiを編集する ------------------- //
void Emitter::EditImGui() {
#ifdef _DEBUG

	if (ImGui::TreeNode(emitterName_.c_str())) {
		ImGui::DragScalar("frameCreateCount", ImGuiDataType_U32, &frameCreateCount_);
		ImGui::DragFloat3("centerPos", &centerPos_.x, 0.01f);
		ImGui::DragFloat3("direction", &direction_.x, 0.01f);
		ImGui::DragFloat3("range", &range_.x, 0.01f);
		ImGui::DragFloat("speed", &speed_, 0.1f);
		ImGui::DragFloat("angle", &angle_, 1.0f, 0.0f, 180.0f);
		ImGui::DragScalar("createTime", ImGuiDataType_U32, &createTime_);
		ImGui::DragScalar("createCount", ImGuiDataType_U32, &createCount_);
		ImGui::DragScalar("lifeTime", ImGuiDataType_U32, &lifeTime_);
		ImGui::DragScalar("emitCount", ImGuiDataType_U32, &emitCount_);
		PolygonMeshManager::MeshListGui(useObjName_);

		// 保存したいEmitterの名前を登録
		char buffer[128];
		// std::stringからバッファにコピー
		strncpy_s(buffer, emitterName_.c_str(), sizeof(buffer));
		// ImGuiで文字を入力
		if (ImGui::InputText("Emitter", buffer, IM_ARRAYSIZE(buffer))) {
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
			AddItem(emitterName_, "createCount", createCount_);
			AddItem(emitterName_, "lifeTime", lifeTime_);
			AddItem(emitterName_, "emitCount", emitCount_);
			AddItem(emitterName_, "speed", speed_);
			AddItem(emitterName_, "radius", radius_);
			AddItem(emitterName_, "angle", angle_);
			AddItem(emitterName_, "useObjectName", useObjName_);
			AddItem(emitterName_, "emitterName", emitterName_);
			// 保存する
			SaveFileEmitter(emitterName_);
			std::string message = std::format("{}.json saved.", emitterName_);
			//MessageBoxA(nullptr, message.c_str(), "Adjustment", 0);
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

			// std::string型
		} else if (std::holds_alternative<std::string>(item.value)) {
			root[groupName][itemName] = std::get<std::string>(item.value);
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
	std::string filePath = kDirectoryPath_ + groupName;
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

			// std::string
		} else if (itItem->is_string()) {
			std::string name = itItem->get<std::string>();
			SetValue(groupName, itemName, name);
		}
	}

	centerPos_ = GetValue<Vector3>(groupName, "centerPos");
	direction_ = GetValue<Vector3>(groupName, "direction");
	range_ = GetValue<Vector3>(groupName, "range");;

	lifeTime_ = GetValue<uint32_t>(groupName, "lifeTime");
	createTime_ = GetValue<uint32_t>(groupName, "createTime");
	createCount_ = GetValue<uint32_t>(groupName, "createCount");
	emitCount_ = GetValue<uint32_t>(groupName, "emitCount");

	speed_ = GetValue<float>(groupName, "speed");
	radius_ = GetValue<float>(groupName, "radius");
	angle_ = GetValue<float>(groupName, "angle");

	useObjName_ = GetValue<std::string>(groupName, "useObjectName");
	emitterName_ = GetValue<std::string>(groupName, "emitterName");

}

Vector3 Emitter::GetPointInCone(const float& theta, const float& radius, const Vector3& direction, const Vector3& origine) {
	// 円錐の軸上の点を計算
	Vector3 point;
	point = origine + direction.normalize();

	// 基底ベクトルを計算(方向ベクトルに垂直な2つのベクトルを作成)
	Vector3 vertical = Vector3::CrossProduct({ 1,0,0 }, direction);
	if (Vector3::DotProduct(vertical, direction) == 0) {
		vertical = Vector3::CrossProduct({ 0,1,0 }, direction);
	}
	Vector3 underVector = Vector3::Normalize(vertical);
	if (underVector.length() < 1e-6) {
		underVector = Vector3::CrossProduct({ 0,1,0 }, direction).normalize();
	}

	Vector3 vVector = direction.CrossProduct(underVector, direction).normalize();

	// 円周上の点を計算
	Vector3 circlePoint = (underVector * std::cos(theta) + vVector * std::sin(theta)) * radius;

	return point + circlePoint;
}

Vector3 Emitter::GetEmitDirectionRandom(const float& radius, const Vector3& direction) {
	float minAngle = -angle_ * ToRadian;
	float maxAngle = angle_ * ToRadian;
	float randomRoll = RandomFloat(minAngle, maxAngle);
	float randomPitch = RandomFloat(minAngle, maxAngle);
	float randomYaw = RandomFloat(minAngle, maxAngle);

	Quaternion randomQuaternion = Quaternion::EulerRadian(Vector3{ randomPitch,randomYaw, randomRoll });
	return direction * randomQuaternion;
}
