#pragma once
#include <memory>
#include "Engine/Game/Scene/BaseScene.h"
#include "Engine/Game/Camera/Camera3D.h"
#include "Game/Input/Input.h"
#include "Game/Scene/TitleScene.h"
#include "Engine/Render/RenderPath/RenderPath.h"
#include "Engine/Render/RenderNode/Object3DNode/Object3DNode.h"
#include "Engine/Render/RenderNode/Sprite/SpriteNode.h"
#include "Engine/Render/RenderNode/Outline/OutlineNode.h"
#include "Engine/DirectX/DirectXSwapChain/DirectXSwapChain.h"
#include "Game/UI/FadePanel.h"
#include "Engine/Game/GameTimer/GameTimer.h"

/// <summary>
/// ゲームクリア時に描画する
/// </summary>
class ClearScene :
    public BaseScene {
public:

    ClearScene() = default;
    ~ClearScene() = default;

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

	std::unique_ptr<FadePanel> fadePanel_ = nullptr;

	// ------------------- RenderNode ------------------- //
	std::shared_ptr<Object3DNode> object3DNode;
	std::shared_ptr<SpriteNode> spriteNode;
	std::shared_ptr<OutlineNode> outlineNode;
	RenderPath path;
};

