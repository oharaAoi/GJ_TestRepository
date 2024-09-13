#pragma once
// STL
#include "stdint.h"
#include <iostream>
#include <fstream>
#include <variant>
#include <map>
#include <unordered_map>
#include <string>
// Math
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Definition.h"
#include "Engine/Math/Quaternion.h"
#include "Game/MyRandom.h"
// GameObject
#include "Game/Effect/Particle.h"
// Manager
#include "Engine/Game/Managers/PolygonMeshManager/PolygonMeshManager.h"
// JSON
#include "externals/nlohmann/json.hpp"

using json = nlohmann::json;

class EffectManager;

/// <summary>
/// Particleを出現させるクラス
/// </summary>
class Emitter {
public:

	// Emitterの調整項目
	struct Item {
		std::variant<uint32_t, float, Vector3, bool, std::string> value;
	};

	struct Group {
		std::map<std::string, Item> items;
	};

public:

	Emitter(EffectManager* effectManager, const std::string& emitterName,
			const Vector3& centerPos, const Vector3& direction);
	~Emitter();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init(EffectManager* effectManager, const std::string& emitterName,
			  const Vector3& centerPos, const Vector3& direction);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw() const;

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////
	
	/// <summary>
	/// Particleを放出する関数
	/// </summary>
	void Emit();

	/// <summary>
	/// ImGuiで編集する関数
	/// </summary>
	void EditImGui();

	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName"></param>
	void CreateGroup(const std::string& groupName);

	/// <summary>
	/// Emitterの情報を保存する
	/// </summary>
	void SaveFileEmitter(const std::string& groupName);

	/// <summary>
	/// Emitterの情報を読み込む
	/// </summary>
	void LoadEmitter(const std::string& groupName);

	Vector3 GetPointInCone(const float& theta, const float& radius, const Vector3& direction, const Vector3& origine);

	Vector3 GetEmitDirectionRandom(const float& radius, const Vector3& direction);

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　accessor
//////////////////////////////////////////////////////////////////////////////////////////////////

	void GetEffectManager(EffectManager* effectManager) { effectManager_ = effectManager; };

	const bool GetIsDead() const { return isDead_; }

	/// <summary>
	/// keyで指定した値をgroupに保存する
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="groupName">: グループの名前</param>
	/// <param name="key">: 項目</param>
	/// <param name="value">: 値</param>
	template<typename T>
	void SetValue(const std::string& groupName, const std::string& key, const T& value);

	/// <summary>
	/// groupにkeyと値を追加する
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="groupName">: グループの名前</param>
	/// <param name="key">: 項目</param>
	/// <param name="value">: 値</param>
	template<typename T>
	void AddItem(const std::string& groupName, const std::string& key, const T& value);

	/// <summary>
	/// grounpからkeyで指定した値を取得する
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="groupName">: グループの名前</param>
	/// <param name="key">: 項目</param>
	/// <returns>: uint32_t, float, Vector3, bool, vector.Vector3 </returns>
	template<typename T>
	T GetValue(const std::string& groupName, const std::string& key, const T& defaultValue = T()) const;

private:

	EffectManager* effectManager_ = nullptr;

	bool isDead_ = false;			// Emitterの寿命

	// ------------------- emitterを保存する際に使用する変数 ------------------- //
	std::map<std::string, Group> emitterData_;

	const std::string kDirectoryPath_ = "./Game/Resources/Effect/Emitter/";

	// ------------------- emitterに関する変数 ------------------- //
	Vector3 centerPos_ = {0,0,0};			// emitterの中心位置
	Vector3 direction_ = { 0,1,0 };			// emitterがどの方向を向いているか
	Vector3 range_ = { 1,1,1 };				// emitterのサイズ(生成する範囲)
	
	uint32_t createTime_ = 120;		// 何フレームごとに生成するか
	uint32_t frameCreateCount_ = 0;	// 1フレームに何個生成するか
	uint32_t createCount_ = 1;		// 生成時に何個生成するか
	uint32_t lifeTime_ = 120;		// particleの生存時間
	uint32_t emitCount_ = 1;		// particleを放出する回数

	float speed_ = 1;				// particleの速度
	float radius_  = 1;				// particleが発射される範囲
	float angle_ = 30;				// どのくらい方向から発射されるか

	std::string useObjName_;	// 使用するobjファイルの名前
	std::string emitterName_;	// emitterの名前

	// ------------------- particleに関する変数 ------------------- //
	Vector3 particleTranslation_;	// particleの位置
	Vector3 particleScale_;			// particleの大きさ
	Vector3 particleVelocity_;		// particleの速度

};

//==================================================================================
// ↓　アイテムをセットする
//==================================================================================

template<typename T>
inline void Emitter::SetValue(const std::string& groupName, const std::string& key, const T& value) {
	// グループの参照を取得
	Group& group = emitterData_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

template<typename T>
inline void Emitter::AddItem(const std::string& groupName, const std::string& key, const T& value) {
	Group& group = emitterData_[groupName];
	if (group.items.find(key) == group.items.end()) {
		SetValue(groupName, key, value);
	}
}

template<typename T>
inline T Emitter::GetValue(const std::string& groupName, const std::string& key, const T& defaultValue) const {
	// 未登録チェック
	assert(emitterData_.find(groupName) != emitterData_.end());
	// グループの参照を取得
	const Group& group = emitterData_.at(groupName);

	auto it = group.items.find(key);
	if (it != group.items.end()) {
		// 存在すればその値を返す
		const Item& item = it->second;
		return std::get<T>(item.value);
	}

	// 存在しない場合はデフォルト値を返す
	return defaultValue;
}
