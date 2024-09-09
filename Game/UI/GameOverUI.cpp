#include "GameOverUI.h"

GameOverUI::GameOverUI() {
}

GameOverUI::~GameOverUI() {
}

void GameOverUI::Init() {
	
}

void GameOverUI::Update() {
	
}

void GameOverUI::Begin_Rendering() {
	goTitle.begin_rendering();
	goGame.begin_rendering();
}

void GameOverUI::Draw() const {
	goTitle.draw();
	goGame.draw();
}

#ifdef _DEBUG
#include <externals/imgui/imgui.h>
void GameOverUI::Debug_gui() {
	ImGui::Begin("SelectUI");
	goTitle.debug_gui();
	goGame.debug_gui();
	ImGui::End();
}
#endif // _DEBUG