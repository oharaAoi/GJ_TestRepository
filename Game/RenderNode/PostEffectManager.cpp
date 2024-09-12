#include "PostEffectManager.h"

#include <Engine/Game/GameTimer/GameTimer.h>
#include <Engine/Math/Definition.h>

#include "Game/GameCharacter/Boss.h"

void PostEffectManager::initialize(const std::shared_ptr<VignetteNode>& vignetteNode_) {
	vignetteNode = vignetteNode_;
	vignetteNode->set_level(0);
	isActiveVignette = false;
	vignetteTimer = 0;
	vignetteResetCall = {
		std::bind(&PostEffectManager::restart_vignette, this), 0
	};
}

void PostEffectManager::update() {
	constexpr float VignetteActivateHight = 7;
	vignetteResetCall.update();
	if (!vignetteResetCall.is_finished()) {
		vignetteTimer += GameTimer::DeltaTime();
		vignetteNode->set_level(
			1 - (std::cos(vignetteTimer * PI2) + 1) / 2
		);
	}
	float bossWorldY = boss->world_position().y;
	if (isActiveVignette) {
		if (bossWorldY < VignetteActivateHight) {
			stop_vignette();
		}
	}
	else {
		if (bossWorldY >= VignetteActivateHight) {
			active_vignette();
		}
	}
}

void PostEffectManager::restart_vignette() {
	if (isActiveVignette) {
		if (vignetteResetCall.is_finished()) {
			vignetteResetCall.restart(1);
			vignetteTimer = 0;
		}
	}
}

void PostEffectManager::active_vignette() {
	isActiveVignette = true;
	restart_vignette();
}

void PostEffectManager::stop_vignette() {
	isActiveVignette = false;
}

void PostEffectManager::set_boss(const Boss* boss_) {
	boss = boss_;
}

#ifdef _DEBUG
#include <externals/imgui/imgui.h>
void PostEffectManager::debug_gui() {
	ImGui::Begin("PostEffect");
	if (ImGui::Checkbox("Vignette", &isActiveVignette)) {
		if (isActiveVignette) {
			active_vignette();
		}
		else {
			stop_vignette();
		}
	}
	vignetteNode->debug_gui();
	ImGui::End();
}
#endif // _DEBUG
