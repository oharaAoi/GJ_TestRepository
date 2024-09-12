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

	makimonoOrigine_UI = std::make_unique<UIObject>("scrollStart.png", ancherPoint);
	makimonoEnd_UI = std::make_unique<UIObject>("scrollEnd.png", ancherPoint);

	makimono_UI = std::make_unique<UIObject>("scroll.png", ancherPoint);
}

void TutorialUI::Update() {
	content_UI->Update({1.0f,1.0f}, {250, 300});
	woodBord_UI->Update({ 1.0, 1.0f }, { 640, 87 });
}

void TutorialUI::BeginRendering() {
	content_UI->begin_rendering();
	woodBord_UI->begin_rendering();
	/*makimonoOrigine_UI->begin_rendering();
	makimonoEnd_UI->begin_rendering();*/
	makimono_UI->begin_rendering();
}

void TutorialUI::Draw() const {
	content_UI->draw();
	woodBord_UI->draw();
	/*makimonoOrigine_UI->draw();
	makimonoEnd_UI->draw();*/
	makimono_UI->draw();
}

#ifdef _DEBUG
void TutorialUI::EditGui() {
	woodBord_UI->debug_gui();
	makimono_UI->debug_gui();
}
#endif // DEBUG