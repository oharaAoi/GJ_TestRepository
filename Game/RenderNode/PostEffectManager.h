#pragma once

#include <memory>

#include "Game/RenderNode/VignetteNode.h"
#include "Engine/Render/RenderNode/ChromaticAberration/ChromaticAberrationNode.h"
#include <Engine/Utility/TimedCall/TimedCall.h>

class Boss;
class Player;

class PostEffectManager {
public:
	PostEffectManager() = default;
	~PostEffectManager() = default;

public:
	void initialize(
		const std::shared_ptr<VignetteNode>& vignetteNode,
		const std::shared_ptr<ChromaticAberrationNode>& chromaticAberrationNode
	);
	void update();

private:
	void restart_vignette();
	void vignette_update();
	void active_vignette();
	void stop_vignette();

	void aberration_update();

public:
	void set_boss(const Boss* boss_);
	void set_player(const Player* player_);

#ifdef _DEBUG
public:
	void debug_gui();
#endif // _DEBUG

private:
	bool isActiveVignette;
	float vignetteTimer;
	TimedCall<void(void)> vignetteResetCall;
	std::shared_ptr<VignetteNode> vignetteNode;
	std::shared_ptr<ChromaticAberrationNode> chromaticAberrationNode;

	const Boss* boss;
	const Player* player;
};
