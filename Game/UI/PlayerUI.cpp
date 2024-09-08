#include "PlayerUI.h"
#include "externals/imgui/imgui.h"

PlayerUI::PlayerUI() {
	Init();
}

PlayerUI::~PlayerUI() {
}

void PlayerUI::Init() {
}

void PlayerUI::Update() {
	/*ImGui::Begin("PlayerUI", nullptr);
	playerUI.debug_gui();
	ImGui::End();*/
}

void PlayerUI::BeginRendering(){
	playerUI.begin_rendering();
}

void PlayerUI::Draw() const {
	playerUI.draw();
}
