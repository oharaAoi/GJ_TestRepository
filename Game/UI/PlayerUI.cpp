#include "PlayerUI.h"
#include "externals/imgui/imgui.h"

PlayerUI::PlayerUI() {
	Init();
}

PlayerUI::~PlayerUI() {
}

void PlayerUI::Finalize() {

}

void PlayerUI::Init() {
	
}

void PlayerUI::Update(const Vector3& playerPos, const Matrix4x4& vpMatrix) {
	// playerのスクリーン座標を算出する
	// viewport行列
	Matrix4x4 matView = MakeViewportMatrix(0, 0, static_cast<float>(WinApp::GetClientWidth()), static_cast<float>(WinApp::GetClientHight()), 0, 1);
	// view * projection * viewport
	//Matrix4x4 matViewprojection = vpMatrix * 
}

void PlayerUI::BeginRendering(){
	playerControl_UI.begin_rendering();
}

void PlayerUI::Draw() const {
	playerControl_UI.draw();
}
#ifdef _DEBUG
#include <externals/imgui/imgui.h>
void PlayerUI::EditGui() {
	ImGui::Begin("playerUI");
	playerControl_UI.debug_gui();
	ImGui::End();
}

Matrix4x4 PlayerUI::MakeViewportMatrix(const float& left, const float& top, const float& width,
								  const float& height, const float& minDepth, const float& maxDepth) {
	Matrix4x4 result{};
	result[0][0] = width / 2.0f;
	result[1][1] = -height / 2.0f;
	result[2][2] = maxDepth - minDepth;
	result[3][0] = left + (width / 2.0f);
	result[3][1] = top + (height / 2.0f);
	result[3][2] = minDepth;
	result[3][3] = 1.0f;
	return result;
}
#endif
