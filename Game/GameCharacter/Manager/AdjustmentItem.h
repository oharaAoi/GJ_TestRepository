#pragma once
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

	// 軌道の点
	struct Item {
		std::variant<uint32_t, float, Vector3, bool, std::vector<Vector3>> value;
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

	void Init() {};
	void Update() {};


private:


};

