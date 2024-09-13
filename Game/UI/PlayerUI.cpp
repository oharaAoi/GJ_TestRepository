#include "PlayerUI.h"
#include "externals/imgui/imgui.h"
#include "Engine/Game/Transform2D/Transform2D.h"

PlayerUI::PlayerUI() {
	Init();
}

PlayerUI::~PlayerUI() {
}

void PlayerUI::Finalize() {

}

void PlayerUI::Init() {
	playerControl_UI = std::make_unique<UIObject>( "UI1.png", Vector2{ 0.5f,0.5f } );
	playerAttack_UI = std::make_unique<UIObject>( "UI2.png", Vector2{ 0.5f,0.5f } );
	offsetPos_ = { 105.0f,-50.0f };
}

void PlayerUI::Update(const Vector3& playerPos, const Matrix4x4& vpMatrix, const bool& isAttack) {
	// playerのスクリーン座標を算出する
	// viewport行列
	Matrix4x4 matView = MakeViewportMatrix(0, 0, static_cast<float>(WinApp::GetClientWidth()), static_cast<float>(WinApp::GetClientHight()), 0, 1);
	// view * projection * viewport
	Matrix4x4 matViewprojection = vpMatrix * matView;
	Vector3 screenPos = Transform(playerPos, matViewprojection);

	Vector2 playerScreenPosition = { screenPos.x , screenPos.y };

	isPlayerAttack_ = isAttack;
	if (isPlayerAttack_) {
		playerAttack_UI->Update(Vector2{ 0.5f,0.5f }, playerScreenPosition + offsetPos_);
	} else {
		playerControl_UI->Update(Vector2{ 0.5f,0.5f }, playerScreenPosition + offsetPos_);
	}
}

void PlayerUI::BeginRendering() {
	if (isPlayerAttack_) {
		playerAttack_UI->begin_rendering();
	} else {
		playerControl_UI->begin_rendering();
	}
}

void PlayerUI::Draw() const {
	if (isPlayerAttack_) {
		playerAttack_UI->draw();
	} else {
		playerControl_UI->draw();
	}
}
#ifdef _DEBUG
#include <externals/imgui/imgui.h>
void PlayerUI::EditGui() {
	playerControl_UI->debug_gui();
	ImGui::DragFloat2("ui_offset", &offsetPos_.x);
}
#endif

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
Vector3 PlayerUI::Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix[0][0] + vector.y * matrix[1][0] + vector.z * matrix[2][0] + 1.0f * matrix[3][0];
	result.y = vector.x * matrix[0][1] + vector.y * matrix[1][1] + vector.z * matrix[2][1] + 1.0f * matrix[3][1];
	result.z = vector.x * matrix[0][2] + vector.y * matrix[1][2] + vector.z * matrix[2][2] + 1.0f * matrix[3][2];
	float w = vector.x * matrix[0][3] + vector.y * matrix[1][3] + vector.z * matrix[2][3] + 1.0f * matrix[3][3];

	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}
