#include "TutorialUI.h"

TutorialUI::TutorialUI() {
	Init();
}

TutorialUI::~TutorialUI() {
}

void TutorialUI::Finalize() {
}

void TutorialUI::Init() {
	content_UI = std::make_unique<UIObject>("UI_kari.png", ancherPoint);
	woodBord_UI = std::make_unique<UIObject>("woodBoard.png", ancherPoint);
}

void TutorialUI::Update() {
	content_UI->Update({1.0f,1.0f}, {250, 300});
}

void TutorialUI::BeginRendering() {
	content_UI->begin_rendering();
}

void TutorialUI::Draw() const {
	content_UI->draw();
}

#ifdef _DEBUG
void TutorialUI::EditGui() {
	content_UI->debug_gui();
}
#endif // DEBUG