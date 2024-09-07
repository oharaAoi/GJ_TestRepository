#pragma once
#include <string>
#include <list>
#include <memory>
#include <variant>
#include <fstream>
#include <iostream>
#include <filesystem>
#include "Game/GameCharacter/Enemy.h"
#include "Engine/Game/Camera/Camera3D.h"
#include "externals/nlohmann/json.hpp"

using json = nlohmann::json;

/// <summary>
/// Enemyの生成を管理するクラス
/// </summary>
class EnemyManager {
public:	// データ構造体

	struct SettingData {
		Vector3 position;
		EnemyType enemyType;
	};

	struct Items {
		std::variant<uint32_t, float, Vector3, SettingData> value;
	};

public:

	EnemyManager();
	~EnemyManager();

public:	// メンバ関数

	void Init();
	void Update();

#ifdef _DEBUG

	void Begin_Rendering(const Camera3D& camera3D);

	void Draw() const;

	void EditImGui();

	void CreateConfigGui();

	void EditConfigGui();

	void SaveFile();

#endif // _DEBUG

	void LoadFile();

	template<typename T>
	void SetValue(const std::string& groupName, const std::string& key, const T& value);

private:

	const std::string kDirectoryPath_ = "./Engine/Resources/EnemyData/";

	std::map<std::string, Items> loadData_;

#ifdef _DEBUG

	char fileNameBuffer_[36] = "";
	std::string createFileName_;

	std::string currentFileName_;

	std::list<std::unique_ptr<Enemy>> enemyList_;
	std::list<SettingData> settingData_;

	Vector3 createPos_;
	EnemyType createType_;

#endif // _DEBUG

};

template<typename T>
inline void EnemyManager::SetValue(const std::string& groupName, const std::string& key, const T& value) {
	Items newItem{};
	newItem.value = value;
	// 設定した項目をstd::mapに追加
	loadData_[key] = newItem;
}
