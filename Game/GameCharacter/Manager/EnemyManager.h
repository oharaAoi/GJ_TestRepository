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

class GameScene;

/// <summary>
/// Enemyの生成を管理するクラス
/// </summary>
class EnemyManager {
public:	// データ構造体

	struct SettingData {
		Vector3 position;
		EnemyType enemyType;

		SettingData(const Vector3& pos = Vector3{0,0,0}, const EnemyType& type = EnemyType::Normal_Type) {
			position = pos;
			enemyType = type;
		}
	};

	struct Group {
		std::map<std::string, SettingData> items;
	};

public:

	EnemyManager(GameScene* gameScene);
	~EnemyManager();

public:	// メンバ関数

	void Init(GameScene* gameScene);
	void Update(const Vector3& playerPosition);

#ifdef _DEBUG

	void Begin_Rendering(const Camera3D& camera3D);

	void Draw() const;

	void EditImGui();

	void CreateConfigGui();

	void EditConfigGui();

	void SaveFile();

#endif // _DEBUG

	void AddEnemy(const Vector3& positoin, const EnemyType& type);

	void LoadFileName();

	void LoadFile(const std::string& fileName);

private:

	GameScene* gameScene_ = nullptr;

	const std::string kDirectoryPath_ = "./Engine/Resources/EnemyData/";

	std::map<std::string, Group> loadData_;

	std::vector<std::string> fileNameArray_;

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

