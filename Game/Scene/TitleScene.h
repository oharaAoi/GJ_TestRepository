#pragma once
#include <memory>
#include "Engine/Game/Scene/BaseScene.h"
#include "Engine/Game/Camera/Camera3D.h"
#include "Engine/Game/Camera/Camera2D.h"
#include "Game/Input/Input.h"
#include "Game/Scene/TutorialScene.h"
#include "Engine/Render/RenderPath/RenderPath.h"
#include "Engine/Render/RenderNode/Object3DNode/Object3DNode.h"
#include "Engine/Render/RenderNode/Sprite/SpriteNode.h"
#include "Engine/Render/RenderNode/Outline/OutlineNode.h"
#include "Engine/DirectX/DirectXSwapChain/DirectXSwapChain.h"
#include "Game/UI/FadePanel.h"
#include "Engine/Game/GameTimer/GameTimer.h"
#include "Engine/Game/Audio/AudioPlayer.h"
#include "Game/Easing.h"

/// <summary>
/// TitleScene
/// </summary>
class TitleScene :
    public BaseScene {

public:

    TitleScene() = default;
    ~TitleScene() = default;

	void finalize()override;

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

	void FallTitle();

private:

	Input* input_ = nullptr;

	bool isFall_ = true;

	float frameCount_ = 0;
	float frameTime_ = 120 * GameTimer::DeltaTime();
	float t = 0;
	
	std::unique_ptr<Camera3D> camera3D_ = nullptr;

	// ------------------- object ------------------- //
	std::unique_ptr<GameObject> titleObject_ = nullptr;
	std::unique_ptr<GameObject> skydome_ = nullptr;

	// ------------------- sprite ------------------- //

	std::unique_ptr<FadePanel> fadePanel_ = nullptr;

	// ------------------- Audio ------------------- //
	std::unique_ptr<AudioPlayer> start_SE_ = nullptr;
	std::unique_ptr<AudioPlayer> title_BGM_ = nullptr;

	// ------------------- RenderNode ------------------- //
	std::shared_ptr<Object3DNode> object3DNode;
	std::shared_ptr<SpriteNode> spriteNode;
	std::shared_ptr<OutlineNode> outlineNode;
	RenderPath path;

};

