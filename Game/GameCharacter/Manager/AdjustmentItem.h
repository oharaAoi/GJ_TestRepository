#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <string>
#include <variant>
#include "Engine/Math/Vector3.h"

#include "externals/nlohmann/json.hpp"


using json = nlohmann::json;

// グローバル変数の保存先ファイルパス
const std::string kDirectoyPath = "./Game/Resources/AdjustmentItem";

/// <summary>
/// 調整項目を設定するクラス
/// </summary>
class AdjustmentItem {

public: // 構造体

	struct Item {
		std::variant<uint32_t, float, Vector3, bool> value;
	};

	struct Group {
		std::map<std::string, Item> items;
	};

public:

	AdjustmentItem() = default;
	~AdjustmentItem() = default;

	// シングルトン化
	AdjustmentItem(const AdjustmentItem&) = delete;
	AdjustmentItem& operator=(const AdjustmentItem&) = delete;

	static AdjustmentItem* GetInstance();

	void Init(const std::string& nowScene);
	void Update();

	void CreateGroup(const std::string& groupName);
	void LoadAllFiles();
	void LoadFile(const std::string& groupName);
	void SaveFile(const std::string& groupName);

	template<typename T>
	void SetValue(const std::string& groupName, const std::string& key, const T& value);
	template<typename T>
	void AddItem(const std::string& groupName, const std::string& key, const T& value);
	template<typename T>
	T GetValue(const std::string& groupName, const std::string& key) const;

private:

	// 全データ
	std::map<std::string, Group> data_;

	std::string nowSceneName_;

};

template<typename T>
inline void AdjustmentItem::SetValue(const std::string& groupName, const std::string& key, const T& value) {
	// グループの参照を取得
	Group& group = data_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	group.items[key] = newItem;
}

template<typename T>
inline void AdjustmentItem::AddItem(const std::string& groupName, const std::string& key, const T& value) {
	Group& group = data_[groupName];
	if (group.items.find(key) == group.items.end()) {
		SetValue(groupName, key, value);
	}
}

template<typename T>
inline T AdjustmentItem::GetValue(const std::string& groupName, const std::string& key) const {
	// 未登録チェック
	assert(data_.find(groupName) != data_.end());
	// グループの参照を取得
	const Group& group = data_.at(groupName);

	assert(group.items.find(key) != group.items.end());
	const Item& item = group.items.at(key);

	return std::get<T>(item.value);
}