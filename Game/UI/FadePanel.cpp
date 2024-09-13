#include "FadePanel.h"

#include <algorithm>

FadePanel::FadePanel() {
	Init();
}

FadePanel::~FadePanel() {
}

void FadePanel::Init() {
	fadePanel_ = std::make_unique<UIObject>("Fade_Panel.png", Vector2{ 0.0f,0.0f });

	isFade_ = false;
	isFadeFisnish_ = false;

	fadeTime_ = 60;

	moveT_ = 0;
}

void FadePanel::Update() {
	if (isFade_) {
		switch (fadeType_) {
		case FadeType::Fade_In:
			FadeIn();
			break;
		case FadeType::Fade_Out:
			FadeOut();
			break;
		}
	}

	fadePanel_->Update({ 1.0f,1.0f }, { 0,0 }, panelAlpha_);

}

void FadePanel::Begin_Rendering() {
	fadePanel_->begin_rendering();
}

void FadePanel::Draw() const {
	fadePanel_->draw();
}

void FadePanel::SetFadeFadeStart(const FadeType& type) {
	isFade_ = true;
	isFadeFisnish_ = false;
	fadeType_ = type;
	frameCount_ = 0;
	moveT_ = 0;
}

void FadePanel::FadeIn() {
	if (moveT_ < 1.0f) {
		moveT_ += 1.0f * GameTimer::DeltaTime();
		panelAlpha_ = std::lerp(0.0f, 1.0f, std::clamp(moveT_, 0.0f, 1.0f));
	} else {
		//moveT_ = 0;
		isFade_ = false;
		isFadeFisnish_ = true;
	}
}

void FadePanel::FadeOut() {
	if (moveT_ < 1.0f) {
		moveT_ += 1.0f * GameTimer::DeltaTime();
		panelAlpha_ = std::lerp(1.0f, 0.0f, std::clamp(moveT_, 0.0f, 1.0f));
		//panelAlpha_ = std::lerp(1.0f, 0.0f, moveT_);
	} else {
		//moveT_ = 0;
		isFade_ = false;
		isFadeFisnish_ = true;
	}
}

#ifdef _DEBUG
#include <externals/imgui/imgui.h>
void FadePanel::Debug_gui() {
	ImGui::Text("frameCount: %d / %d", frameCount_, fadeTime_);
}
#endif // _DEBUG
