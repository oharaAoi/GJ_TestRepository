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
#include "Game/Function/TimedCall.h"
// Math
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include "Game/MyRandom.h"

using json = nlohmann::json;

class GameScene;

static const uint32_t kAppearanceTime_ = 300;

class MeteoriteManager {
public: // データ構造体

	struct Adjustment {
		uint32_t rePopTime;
		float rePopWidth;
		uint32_t popNum;
	};

	struct Items {
		std::variant<uint32_t, float, Vector2, Vector3, Adjustment> value;
	};

	struct Group {
		std::map<std::string, Items> items;
		uint32_t spawnedFrameCount = 0;	// 選出されてから数えるカウント
	};

public:

	MeteoriteManager();
	~MeteoriteManager();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update(const Vector3& playerPosition);

#ifdef _DEBUG
	/// <summary>
	/// ImGuiを編集する
	/// </summary>
	void EditImGui();

	/// <summary>
	/// ファイルを作成する
	/// </summary>
	void CreateConfigGui();

	/// <summary>
	/// ファイルを編集する
	/// </summary>
	void EditConfigGui();

#endif

public:	// メンバ関数

	void LoadAllFile();

	/// <summary>
	/// 隕石の情報をファイルに保存する
	/// </summary>
	void SaveFile(const std::string& groupName);

	/// <summary>
	/// ファイルを読み込む
	/// </summary>
	void LoadFile(const std::string& groupName);

	/// <summary>
	/// 配置を選出する関数
	/// </summary>
	void SelectionArrange();

	std::string GetRandomKey();

public:

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

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

private:

	const std::string kDirectoryPath_ = "./Engine/Resources/Meteorite/";

	GameScene* gameScene_ = nullptr;

	// 隕石の情報が格納されたデータ
	std::map<std::string, Group> groupMap_;
	std::vector<Group*> groupRefs_;
	
	std::list<TimedCall> timedCalls_;

	std::list<Meteorite> meteoriteList_;

	// ----------- debug用 ----------- //
	uint32_t rePopTime_ = 0;
	float popWidth_ = 5;
	uint32_t popNum_ = 3;
	Vector3 editMeteoPos_;

	std::list<Meteorite> debugMeteoriteList_;
	std::list<Vector3> popedList_;

	std::string saveFileName_;
	char saveNameBuffe_[30];

	std::vector<std::string> fileNameArray_;
	std::string currentFile_;
};

template<typename T>
inline void MeteoriteManager::SetValue(const std::string& groupName, const std::string& key, const T& value) {
	// グループの参照を取得
	Group& group = groupMap_[groupName];
	// 新しい項目のデータを設定
	Items newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

template<typename T>
inline void MeteoriteManager::AddItem(const std::string& groupName, const std::string& key, const T& value) {
	Group& group = groupMap_[groupName];
	if (group.items.find(key) == group.items.end()) {
		SetValue(groupName, key, value);
	}

}

template<typename T>
inline T MeteoriteManager::GetValue(const std::string& groupName, const std::string& key, const T& defaultValue) const {
	// 未登録チェック
	assert(groupMap_.find(groupName) != groupMap_.end());
	// グループの参照を取得
	const Group& group = groupMap_.at(groupName);

	auto it = group.items.find(key);
	if (it != group.items.end()) {
		// 存在すればその値を返す
		const Items& item = it->second;
		return std::get<T>(item.value);
	}

	// 存在しない場合はデフォルト値を返す
	return defaultValue;
}
