#include "Field.h"
#include "Game/Enviroment.h"

Field::Field() {
	Init();
}

Field::~Field() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Field::Init() {
	reset_object("Field.obj");
	velocityY_ = 0.0f;
	accelerationY_ = 0.4f;

	sinkLimitHight_ = 3.0f;

	cylinderHight_ = 12.5f;
	surfaceHight_ = 12.5f;

	isSink_ = false;
	isFloatUp_ = false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Field::Update() {
	Move();
}

void Field::Move() {
	Vector3 translate = transform->get_translate();
	translate.y += velocityY_ * kDeltaTime;

	if (translate.y > 0) {
		translate.y = 0;
		velocityY_ = 0;
	}

	velocityY_ += 0.1f;
	surfaceHight_ = translate.y + cylinderHight_;
	transform->set_translate(translate);
}


#ifdef _DEBUG
#include <externals/imgui/imgui.h>
void Field::EditImGui() {
	ImGui::Begin("Field");
	ImGui::DragFloat("radius", &radius_, 0.1f);
	ImGui::DragFloat("sinkValue", &sinkValue_, 0.1f);
	debug_gui();
	ImGui::End();
}
#endif // _DEBUG
