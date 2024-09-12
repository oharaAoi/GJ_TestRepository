#pragma once
#include <cmath>
#include "Engine/Game/GameObject/SpriteObject.h"
#include "Game/UI/UIObject.h"

enum class FadeType {
	Fade_In,
	Fade_Out
};

/// <summary>
/// シーンフェードのパネル
/// </summary>
class FadePanel {
public:

	FadePanel();
	~FadePanel();

	void Init();

	void Update();

	void Begin_Rendering();

	void Draw() const;

#ifdef _DEBUG
	void Debug_gui();
#endif // _DEBUG

	void SetFadeFadeStart(const FadeType& type);

	void FadeIn();
	void FadeOut();

	const uint32_t GetFadeTime() const { return fadeTime_; }

	const bool GetIsFade() const { return isFade_; }

	const bool GetIsFadeFinish() const { return isFadeFisnish_; }

private:

	std::unique_ptr<UIObject> fadePanel_ = nullptr;

	bool isFade_ = false;
	bool isFadeFisnish_ = false;

	FadeType fadeType_;
	uint32_t frameCount_ = 0;
	uint32_t fadeTime_ = 60;

	float panelAlpha_ = 0.0f;
};

