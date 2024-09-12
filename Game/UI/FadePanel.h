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

private:

	std::unique_ptr<UIObject> fadePanel_ = nullptr;

	bool isFade_ = false;

	FadeType fadeType_;
	uint32_t frameCount_ = 0;
	uint32_t fadeTime_ = 200;

	float panelAlpha_ = 0.0f;
};

