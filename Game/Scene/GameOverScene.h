#pragma once
#include <memory>
#include "Engine/Game/Scene/BaseScene.h"
#include "Engine/Game/Camera/Camera3D.h"
#include "Game/Input/Input.h"
#include "Game/Scene/TitleScene.h"
#include "Game/Scene/GameScene.h"
#include "Game/UI/GameOverUI.h"
#include "Engine/Game/Camera/Camera2D.h"
#include "Engine/Render/RenderPath/RenderPath.h"
#include "Engine/Render/RenderNode/Object3DNode/Object3DNode.h"
#include "Engine/Render/RenderNode/Sprite/SpriteNode.h"
#include "Engine/Render/RenderNode/Outline/OutlineNode.h"
#include "Engine/DirectX/DirectXSwapChain/DirectXSwapChain.h"
#include "Engine/Render/RenderNode/ChromaticAberration/ChromaticAberrationNode.h"
#include "Game/UI/FadePanel.h"
#include "Engine/Game/GameTimer/GameTimer.h"
#include "Engine/Game/Audio/AudioPlayer.h"
#include "Game/GameCharacter/Boss.h"

class GameOverScene :
    public BaseScene {
public:

    GameOverScene() = default;
    ~GameOverScene() = default;

	void finalize() override;

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

	bool nextGame_ = 0;
	bool preNextGame_ = 0;

	bool isSave_ = false;

	Quaternion bossMoveRotate_;

	std::unique_ptr<GameObject> gameOverTitle_ = nullptr;
	std::unique_ptr<GameObject> skydome_ = nullptr;

	std::unique_ptr<Boss> boss_ = nullptr;

	// ------------------- UI ------------------- //
	std::unique_ptr<GameOverUI> gameOverUI_ = nullptr;

	// ------------------- Sound ------------------- //
	std::unique_ptr<AudioPlayer> start_SE_ = nullptr;
	std::unique_ptr<AudioPlayer> choice_SE_ = nullptr;
	std::unique_ptr<AudioPlayer> gameOver_BGM_ = nullptr;

	// ------------------- RenderNode ------------------- //
	std::shared_ptr<Object3DNode> object3DNode;
	std::shared_ptr<SpriteNode> spriteNode;
	std::shared_ptr<OutlineNode> outlineNode;
	RenderPath path;

	std::unique_ptr<FadePanel> fadePanel_ = nullptr;
};

