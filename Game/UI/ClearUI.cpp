#include "ClearUI.h"

ClearUI::ClearUI() {
	Init();
}

ClearUI::~ClearUI() {
	Finalize();
}

void ClearUI::Finalize() {
}

void ClearUI::Init() {
	goTitle_ = std::make_unique<UIObject>("UI_goTitle.png", Vector2{ 0.5f, 0.5f });
	goGame_ = std::make_unique<UIObject>("UI_goGame.png", Vector2{ 0.5f, 0.5f });
	arrow_ = std::make_unique<UIObject>("UI_arrow.png", Vector2{ 0.5f, 0.5f });
}

void ClearUI::Update(const bool& nextGame) {
	if (nextGame) {
		arrow_->Update({ 1,1 }, { 780, 400 });
	} else {
		arrow_->Update({ 1,1 }, { 780, 540 });
	}

	goTitle_->Update({ 1,1 }, { 640, 540 });
	goGame_->Update({ 1,1 }, { 640, 400 });
}

void ClearUI::Begin_Rendering() {

	goTitle_->begin_rendering();
	goGame_->begin_rendering();
	arrow_->begin_rendering();
}

void ClearUI::Draw() const {
	goTitle_->draw();
	goGame_->draw();
	arrow_->draw();
}

#ifdef _DEBUG
#include <externals/imgui/imgui.h>
void ClearUI::Debug_gui() {
	ImGui::Begin("SelectUI");
	goTitle_->debug_gui();
	goGame_->debug_gui();
	arrow_->debug_gui();
	ImGui::End();
}
#endif // _DEBUG