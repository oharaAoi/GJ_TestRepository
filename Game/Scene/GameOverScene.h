#pragma once
#include <memory>
#include "Engine/Game/Scene/BaseScene.h"
#include "Engine/Game/Camera/Camera3D.h"
#include "Game/Input/Input.h"
#include "Game/Scene/TitleScene.h"
#include "Game/Scene/GameScene.h"

class GameOverScene :
    public BaseScene {
public:

    GameOverScene() = default;
    ~GameOverScene() = default;

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

private:

	Input* input_ = nullptr;

	std::unique_ptr<Camera3D> camera3D_ = nullptr;

};

