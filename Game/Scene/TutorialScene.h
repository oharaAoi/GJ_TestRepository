#pragma once
#include <memory>
#include "Engine/Game/Scene/BaseScene.h"
#include "Engine/Game/Camera/Camera3D.h"
#include "Engine/Game/Camera/Camera2D.h"
#include "Game/GameCharacter/Manager/AdjustmentItem.h"
#include "Game/Input/Input.h"
#include "Game/Scene/GameScene.h"
#include "Game/WorldObject/Field.h"
#include "Game/WorldObject/Meteorite.h"
#include "Game/WorldObject/MeteoriteManager.h"
#include "Game/GameCharacter/Player.h"
#include "Game/GameCharacter/Boss.h"

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

private:

	Input* input_ = nullptr;

	std::unique_ptr<Camera3D> camera3D_ = nullptr;

	// ----------- WorldObject ----------- //
	std::unique_ptr<Field> field_ = nullptr;
	std::list<std::unique_ptr<Meteorite>> meteoriteList_;

	// ----------- GameCharacter ----------- //
	std::unique_ptr<Player> player_ = nullptr;
	std::unique_ptr<Boss> boss_ = nullptr;

	// ----------- Manager ----------- //
	std::unique_ptr<MeteoriteManager> meteoriteManager_ = nullptr;
};

