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
#include "Game/Function/TimedCall.h"
#include "Game/MyRandom.h"
#include "Engine/Game/Collision/CollisionManager/CollisionManager.h"

using json = nlohmann::json;

class Hierarchy;

/// <summary>
/// Enemyの生成を管理するクラス
/// </summary>
class EnemyManager {
#ifdef _DEBUG
	friend class EnemyPopEditor;
#endif // _DEBUG

public:	// データ構造体

	struct SettingData {
		Vector3 position;
		EnemyType enemyType;

		SettingData(const Vector3& pos = Vector3{ 0,0,0 }, const EnemyType& type = EnemyType::Normal_Type) {
			position = pos;
			enemyType = type;
		}
	};

	struct Group {
		std::map<std::string, SettingData> items;
	};

	struct AddEnemyData {
		Vector3 pos;
		EnemyType type;
	};

public:

	EnemyManager(std::list<std::unique_ptr<Enemy>>& sceneEnemyList,
				 CollisionManager* collisionManager, bool* isPlayerFlagPtr, const Hierarchy& fieldHierarchy);
	~EnemyManager();

public:	// メンバ関数

	void Init();
	void Update(const Vector3& playerPosition);

	uint32_t CheckConstrainToField(); 

	void SelectArrange();

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

	std::optional<EnemyManager::Group> LoadFile(const std::string& fileName);

	void StartPop();

private:
	void PopFromGroup(const Group& group);

private:

	const std::string kDirectoryPath_ = "./Game/Resources/GameScene/EnemyData/";

	std::map<std::string, Group> loadData_;

	std::vector<std::string> fileNameArray_;

	std::list<Test::TimedCall> timedCalls_;

	// 他のクラスが持つポインタ(参照) ----------------------------------------------------
	std::list<std::unique_ptr<Enemy>>& sceneEnemyList_; // シーンが持っている敵のリスト
	CollisionManager* collisionManager_ = nullptr;
	bool* isPlayerFlagPtr_ = nullptr;
	const Hierarchy& fieldHierarchy_;
	// ------------------------------------------------------------------------------

	uint32_t popTime_ = 420;
	uint32_t firstPopCount_ = 0;
	bool isPop_ = false;

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

