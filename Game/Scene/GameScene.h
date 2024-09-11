#pragma once
#include "Engine/Game/Scene/BaseScene.h"
#include "Game/WorldObject/Field.h"
#include "Game/GameCharacter/Player.h"
#include "Game/WorldObject/Meteorite.h"
#include "Game/GameCharacter/Boss.h"
#include "Game/GameCharacter/Enemy.h"
#include "Game/Input/Input.h"
#include "Game/Scene/ClearScene.h"
#include "Game/Scene/GameOverScene.h"
#include "Game/GameCharacter/Manager/AdjustmentItem.h"
#include "Engine/Game/Camera/Camera2D.h"
// Manager
#include "Game/Effect/EffectManager.h"
#include "Game/WorldObject/MeteoriteManager.h"
#include "Game/GameCharacter/Manager/EnemyManager.h"
#include "Engine/Game/Collision/CollisionManager/CollisionManager.h"
#include "Game/FollowCamera.h"
#include "Engine/Render/RenderPathManager/RenderPathManager.h"
#include "Engine/Game/Managers/AudioManager/AudioManager.h"
#include "Engine/Game/Audio/AudioPlayer.h"
#include "Engine/Render/RenderPath/RenderPath.h"
#include "Engine/Render/RenderNode/Object3DNode/Object3DNode.h"
#include "Engine/Render/RenderNode/Sprite/SpriteNode.h"
#include "Engine/DirectX/DirectXSwapChain/DirectXSwapChain.h"
#include "Game/UI/PlayerUI.h"

class EditorController;

class GameScene :
	public BaseScene {
public:
	GameScene();
	~GameScene();

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
	/// 隕石とフィールドの当たり判定を取る
	/// </summary>
	void CheckMeteoToField();

	/// <summary>
	/// 隕石が吸い寄せられるかを判定する
	/// </summary>
	void CheckMeteoAttraction();

	/// <summary>
	/// Bossとの当たり判定を取る
	/// </summary>
	void CheckBossCollision();

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

	// ------------------- UI ------------------- //
	std::unique_ptr<PlayerUI> playerUI_ = nullptr;

	// ------------------- RenderNode ------------------- //
	std::shared_ptr<Object3DNode> object3DNode;
	std::shared_ptr<SpriteNode> spriteNode;
	RenderPath path;

#ifdef _DEBUG
	std::unique_ptr<EditorController> editor;
#endif // _DEBUG
};

