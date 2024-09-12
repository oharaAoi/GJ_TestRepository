#pragma once

#include <memory>

#include "Game/RenderNode/VignetteNode.h"
#include "Engine/Render/RenderNode/Outline/OutlineNode.h"
#include <Engine/Utility/TimedCall/TimedCall.h>

class Boss;

class PostEffectManager {
public:
	PostEffectManager() = default;
	~PostEffectManager() = default;

public:
	void initialize(
		const std::shared_ptr<VignetteNode>& vignetteNode
	);
	void update();

private:
	void restart_vignette();
	void vignnette_update();
	void active_vignette();
	void stop_vignette();

public:
	void set_boss(const Boss* boss_);

#ifdef _DEBUG
public:
	void debug_gui();
#endif // _DEBUG

private:
	bool isActiveVignette;
	float vignetteTimer;
	TimedCall<void(void)> vignetteResetCall;
	std::shared_ptr<VignetteNode> vignetteNode;

	const Boss* boss;
};
