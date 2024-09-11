#pragma once
#include <unordered_map>
#include <memory>
#include "Engine/Game/Scene/BaseScene.h"
#include "Engine/Game/Camera/Camera3D.h"
#include "Engine/Game/Camera/Camera2D.h"
#include "Game/GameCharacter/Manager/AdjustmentItem.h"
#include "Game/Input/Input.h"
#include "Game/Scene/GameScene.h"
#include "Game/WorldObject/Field.h"
#include "Game/WorldObject/Meteorite.h"
#include "Game/GameCharacter/Enemy.h"
#include "Game/WorldObject/MeteoriteManager.h"
#include "Game/GameCharacter/Player.h"
#include "Game/GameCharacter/Boss.h"
#include "Game/GameCharacter/Manager/EnemyManager.h"
#include "Engine/Game/Collision/CollisionManager/CollisionManager.h"
#include "Engine/Render/RenderPath/RenderPath.h"
#include "Engine/Render/RenderNode/Object3DNode/Object3DNode.h"
#include "Engine/Render/RenderNode/Sprite/SpriteNode.h"
#include "Engine/Render/RenderNode/Outline/OutlineNode.h"
#include "Engine/DirectX/DirectXSwapChain/DirectXSwapChain.h"
#include "Game/UI/TutorialUI.h"

/// <summary>
/// 内容
/// </summary>
enum class TutorialContent {
	FirstMove_Content = 0,
	RodPutOn_Content,
	MeteoCollision_Content,
	CantMoveCanRotate_Content,
	FirstEnemy_Content,
	EnemyCollisionToMeteo_Content,
	MeteoAttract_Content
};

/// <summary>
/// チュートリアルを行うクラス
/// </summary>
class TutorialScene :
    public BaseScene {
public:

    TutorialScene() = default;
    ~TutorialScene() = default;

	void initialize() override;

	void finalize() override;

	void load() override;

	void begin() override;
	void update() override;
	void begin_rendering() override;
	void late_update() override;

	void draw() const override;

#ifdef _DEBUG
	void debug_update() override;
#endif // _DEBUG

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

public:	// チュートリアルの内容

	void FirstMoveContent();
	void RodPutOnContent();
	void MeteoCollisionContent();
	void CantMoveCanRotateContent();
	void FirstEnemyContent();
	void EnemyCollisionToMeteoContent();
	void MeteoAttractContent();

	using FunctionPointer = void(TutorialScene::*)();
	std::unordered_map<TutorialContent, FunctionPointer> functionMap_ = {
	{ TutorialContent::FirstMove_Content, &TutorialScene::FirstMoveContent },
	{ TutorialContent::RodPutOn_Content, &TutorialScene::RodPutOnContent },
	{ TutorialContent::MeteoCollision_Content, &TutorialScene::MeteoCollisionContent },
	{ TutorialContent::CantMoveCanRotate_Content, &TutorialScene::CantMoveCanRotateContent },
	{ TutorialContent::FirstEnemy_Content, &TutorialScene::FirstEnemyContent },
	{ TutorialContent::EnemyCollisionToMeteo_Content, &TutorialScene::EnemyCollisionToMeteoContent },
	{ TutorialContent::MeteoAttract_Content, &TutorialScene::MeteoAttractContent }
	};

	void ExecuteTutorialContent(const TutorialContent& content);

private:

	Input* input_ = nullptr;

	std::unique_ptr<Camera3D> camera3D_ = nullptr;

	// ----------- WorldObject ----------- //
	std::unique_ptr<Field> field_ = nullptr;
	
	// ----------- GameCharacter ----------- //
	std::unique_ptr<Player> player_ = nullptr;
	std::unique_ptr<Boss> boss_ = nullptr;

	// ----------- List ----------- //
	std::list<std::unique_ptr<Enemy>> enemyList_;
	std::list<std::unique_ptr<Meteorite>> meteoriteList_;

	// ----------- Manager ----------- //
	std::unique_ptr<MeteoriteManager> meteoriteManager_ = nullptr;
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

	// ------------------- RenderNode ------------------- //
	std::shared_ptr<Object3DNode> object3DNode;
	std::shared_ptr<SpriteNode> spriteNode;
	std::shared_ptr<OutlineNode> outlineNode;
	RenderPath path;

	// ------------------- 内容に関する変数 ------------------- //
	bool isTutorialFinish_;
	TutorialContent content_;
	uint32_t frameCount_ = 0;
	uint32_t attackCount_ = 0;
	uint32_t skipCount_ = 0;
};

