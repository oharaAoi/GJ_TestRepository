#pragma once
// Scene
#include "Engine/Game/Scene/BaseScene.h"
// WorldObject
#include "Game/WorldObject/Field.h"
// GameObject
#include "Game/GameCharacter/Player.h"
// Input
#include "Game/Input/Input.h"

#include "Game/FollowCamera.h"
#include "Engine/Render/RenderPathManager/RenderPathManager.h"

class GameScene :
	public BaseScene {
public:

	GameScene() = default;
	~GameScene() = default;

	void initialize() override;

	void begin() override;
	void update() override;
	void begin_rendering() override;
	void late_update() override;

	void draw() const override;

#ifdef _DEBUG
	void debug_update() override;
#endif // _DEBUG

private:

	// ------------------- WorldObject ------------------- //
	std::unique_ptr<Field> field_ = nullptr;

	// ------------------- GameObject ------------------- //
	std::unique_ptr<Player> player_ = nullptr;

	// ------------------- Camera ------------------- //
	std::unique_ptr<FollowCamera> camera3D_ = nullptr;

};

