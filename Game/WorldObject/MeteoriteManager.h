#pragma once
#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include <list>
#include <variant>
#include <unordered_map>
// JSON
#include "externals/nlohmann/json.hpp"
// Game
#include "Game/WorldObject/Meteorite.h"
// 
#include "Engine/Utility/TimedCall/TimedCall.h"
// Math
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include "Game/MyRandom.h"
#include "Engine/Game/Collision/CollisionManager/CollisionManager.h"

using json = nlohmann::json;

class GameScene;
class EditorController;

static const float kAppearanceTime_ = 8.0f;

class MeteoriteManager {
#ifdef _DEBUG
	friend class MeteoritePopEditor;
#endif // _DEBUG
public: // データ構造体

	struct Adjustment {
		float repopTime;
		float popWidth;
		uint32_t numMeteorites;
	};

	struct Items {
		std::variant<uint32_t, float, Vector2, Vector3, Adjustment> value;
	};

	struct Group {
		std::map<std::string, Items> items;
		float spawnedFrameCount = 0;	// 選出されてから数えるカウント
	};

public:

	MeteoriteManager(std::list<std::unique_ptr<Meteorite>>& sceneMeteoList, CollisionManager* collisionManager);
	~MeteoriteManager();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update(const Vector3& playerPosition);

	void StartPop();

#ifdef _DEBUG
	void SetEditor(const EditorController* editController_);
#endif

public:	// メンバ関数

	/// <summary>
	/// 隕石を追加
	/// </summary>
	void AddMeteo(const Vector3& position);

	void LoadAllFile();

	/// <summary>
	/// 隕石の情報をファイルに保存する
	/// </summary>
	void SaveFile(const std::string& groupName);

	/// <summary>
	/// ファイルを読み込む
	/// </summary>
	std::optional<MeteoriteManager::Group> LoadFile(const std::string& groupName);

	/// <summary>
	/// 配置を選出する関数
	/// </summary>
	void SelectionArrange();

	void PopFromGroup(const MeteoriteManager::Group& group);

	/// <summary>
	/// 数が足りない時に出現させる関数
	/// </summary>
	void PopRadomFromGroup(const MeteoriteManager::Group& group);

	std::string GetRandomKey();


#ifdef _DEBUG
	void DebugGui();
#endif // _DEBUG


public:

	/// <summary>
	/// keyで指定した値をgroupに保存する
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="groupName">: グループの名前</param>
	/// <param name="key">: 項目</param>
	/// <param name="value">: 値</param>
	template<typename T>
	void SetValue(Group& group, const std::string& key, const T& value);

	/// <summary>
	/// groupにkeyと値を追加する
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="groupName">: グループの名前</param>
	/// <param name="key">: 項目</param>
	/// <param name="value">: 値</param>
	template<typename T>
	void AddItem(Group& group, const std::string& key, const T& value);

	/// <summary>
	/// grounpからkeyで指定した値を取得する
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="group">: グループの実体</param>
	/// <param name="key">: 項目</param>
	/// <returns>: uint32_t, float, Vector3, bool, vector.Vector3 </returns>
	template<typename T>
	T GetValue(const Group& group, const std::string& key, const T& defaultValue = T()) const;

private:

	const std::string kDirectoryPath_ = "./Game/Resources/GameScene/Meteorite/";

	uint32_t meteoAddNum_ = 20;

	float lessThanTargetMeteoNum_ = 0;

	// 隕石の情報が格納されたデータ
	std::map<std::string, Group> groupMap_;
	std::vector<Group*> groupRefs_;

	std::list<TimedCall<void(void)>> timedCalls_;

	std::list<Meteorite> meteoriteList_;

	// 他のクラスが持つポインタ(参照) ----------------------------------------------------
	std::list<std::unique_ptr<Meteorite>>& sceneMeteoList_; // シーンが持っている敵のリスト
	CollisionManager* collisionManager_ = nullptr;
	// ------------------------------------------------------------------------------

	bool isStart_ = false;

#ifdef _DEBUG
	// ----------- debug用 ----------- //
	const EditorController* editController;
#endif // _DEBUG

};

template<typename T>
inline void MeteoriteManager::SetValue(Group& group, const std::string& key, const T& value) {
	//// グループの参照を取得
	//Group& group = groupMap_[groupName];
	// 新しい項目のデータを設定
	Items newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

template<typename T>
inline void MeteoriteManager::AddItem(Group& group, const std::string& key, const T& value) {
	//Group& group = groupMap_[groupName];
	if (group.items.find(key) == group.items.end()) {
		SetValue(group, key, value);
	}
}

template<typename T>
inline T MeteoriteManager::GetValue(const Group& group, const std::string& key, const T& defaultValue) const {


	auto it = group.items.find(key);
	if (it != group.items.end()) {
		// 存在すればその値を返す
		const Items& item = it->second;
		return std::get<T>(item.value);
	}

	// 存在しない場合はデフォルト値を返す
	return defaultValue;
}
