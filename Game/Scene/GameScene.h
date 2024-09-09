#pragma once
// Scene
#include "Engine/Game/Scene/BaseScene.h"
// WorldObject
#include "Game/WorldObject/Field.h"
// GameObject
#include "Game/GameCharacter/Player.h"
#include "Game/WorldObject/Meteorite.h"
#include "Game/GameCharacter/Boss.h"
#include "Game/GameCharacter/Enemy.h"
// Input
#include "Game/Input/Input.h"

// Manager
#include "Game/Effect/EffectManager.h"
#include "Game/WorldObject/MeteoriteManager.h"
#include "Game/GameCharacter/Manager/EnemyManager.h"
#include "Engine/Game/Collision/CollisionManager/CollisionManager.h"
#include "Game/FollowCamera.h"
#include "Engine/Render/RenderPathManager/RenderPathManager.h"

class GameScene :
	public BaseScene {
public:

	GameScene() = default;
	~GameScene() = default;

	void initialize() override;

	void load() override;

	void begin() override;
	void update() override;
	void begin_rendering() override;
	void late_update() override;

	void draw() const override;

#ifdef _DEBUG
	void debug_update() override;
#endif // _DEBUG

public:

	/// <summary>
	/// 隕石が吸い寄せられるかを判定する
	/// </summary>
	void CheckMeteoAttraction();

	/// <summary>
	/// Bossとの当たり判定を取る
	/// </summary>
	void CheckBossCollision();

	void CheckEnemyCollison();

	/// <summary>
	/// 隕石をリストに追加する
	/// </summary>
	/// <param name="position"></param>
	void AddMeteorite(const Vector3& position);

	/// <summary>
	/// 敵をリストに追加する
	/// </summary>
	/// <param name="position"></param>
	/// <param name="enemyType"></param>
	void AddEnemy(const Vector3& position, const EnemyType& enemyType);

private:

	uint32_t frameCount_ = 0;

	// ------------------- WorldObject ------------------- //
	std::unique_ptr<Field> field_ = nullptr;

	// ------------------- GameObject ------------------- //
	std::unique_ptr<Player> player_ = nullptr;
	std::unique_ptr<Boss> boss_ = nullptr;
	std::list<std::unique_ptr<Enemy>> enemyList_;
	std::list<std::unique_ptr<Meteorite>> meteoriteList_;

	// ------------------- Camera ------------------- //
	std::unique_ptr<FollowCamera> camera3D_ = nullptr;

	// ------------------- Manager ------------------- //
	EffectManager* effectManager_ = nullptr;
	std::unique_ptr<MeteoriteManager> meteoriteManager_ = nullptr;

	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;

	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

};

