#include "FadePanel.h"

FadePanel::FadePanel() {
	Init();
}

FadePanel::~FadePanel() {
}

void FadePanel::Init() {
	fadePanel_ = std::make_unique<UIObject>("Fade_Panel.png", Vector2{ 0.0f,0.0f });

	isFade_ = false;
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
	fadeType_ = type;
	frameCount_ = 0;
}

void FadePanel::FadeIn() {
	if (++frameCount_ < fadeTime_) {
		float t = static_cast<float>(frameCount_) / static_cast<float>(fadeTime_);
		panelAlpha_ = std::lerp(0.0f, 1.0f, t);
	} else {
		frameCount_ = 0;
		isFade_ = false;
	}
}

void FadePanel::FadeOut() {
	if (++frameCount_ < fadeTime_) {
		float t = static_cast<float>(frameCount_) / static_cast<float>(fadeTime_);
		panelAlpha_ = std::lerp(1.0f, 0.0f, t);
	} else {
		frameCount_ = 0;
		isFade_ = false;
	}
}

#ifdef _DEBUG
void FadePanel::Debug_gui() {
}
#endif // _DEBUG
