#include "Boss.h"
#include "Engine/Game/Transform3D/Transform3D.h"
#include <Game/Enviroment.h>

Boss::Boss() {
	Init();
}

Boss::~Boss() {
}

void Boss::Finalize() {
	bossHit_SE_->finalize();
	fieldPush_SE_->finalize();
}

void Boss::Init() {
	reset_object("mouth.obj");

	overLine_ = std::make_unique<GameObject>();
	overLine_->reset_object("OverLine.obj");

	faceParts_.emplace_back(std::make_unique<GameObject>("bossFace.obj"));
	faceParts_.emplace_back(std::make_unique<GameObject>("lowerJaw.obj"));
	faceParts_.emplace_back(std::make_unique<GameObject>("upperJaw.obj"));
	faceParts_.emplace_back(std::make_unique<GameObject>("InMouth.obj"));
	faceParts_.emplace_back(std::make_unique<GameObject>("bossEyes.obj"));
	faceParts_.emplace_back(std::make_unique<GameObject>("bossEyes.obj"));
	faceParts_.emplace_back(std::make_unique<GameObject>("bossEyebrows.obj"));
	faceParts_.emplace_back(std::make_unique<GameObject>("bossEyebrows.obj"));

	for (uint32_t oi = 0; oi < faceParts_.size(); ++oi) {
		faceParts_[oi]->set_parent(*hierarchy);
	}

	overLine_->set_parent(*hierarchy);

	Vector3 translate = transform->get_translate();
	translate.y = -130.0f;
	transform->set_translate(translate);

	velocity_ = { 0,0.2f,0 };

	pushBackValue_ = 0.0f;
	pushBackStrength_ = 0.2f;

	movingMouth_.parameter = 0;
	movingMouth_.period = 90;
	movingMouth_.amplitude = 0.4f;

	adjustmentItem_ = AdjustmentItem::GetInstance();
	const char* groupName = "Boss";
	adjustmentItem_->AddItem(groupName, "LeftEye", faceParts_[LeftEye_Parts]->get_transform().get_translate());
	adjustmentItem_->AddItem(groupName, "RightEye", faceParts_[RightEye_Parts]->get_transform().get_translate());
	adjustmentItem_->AddItem(groupName, "LeftEyebrows", faceParts_[LeftEyebrows_Parts]->get_transform().get_translate());
	adjustmentItem_->AddItem(groupName, "RightEyebrows", faceParts_[RightEyebrows_Parts]->get_transform().get_translate());

	faceParts_[LeftEye_Parts]->get_transform().set_translate(adjustmentItem_->GetValue<Vector3>(groupName, "LeftEye"));
	faceParts_[RightEye_Parts]->get_transform().set_translate(adjustmentItem_->GetValue<Vector3>(groupName, "RightEye"));
	faceParts_[LeftEyebrows_Parts]->get_transform().set_translate(adjustmentItem_->GetValue<Vector3>(groupName, "LeftEyebrows"));
	faceParts_[RightEyebrows_Parts]->get_transform().set_translate(adjustmentItem_->GetValue<Vector3>(groupName, "RightEyebrows"));

	bossHit_SE_ = std::make_unique<AudioPlayer>();
	bossHit_SE_->initialize("SE_bossHited.wav", 0.5f, false);
	fieldPush_SE_ = std::make_unique<AudioPlayer>();
	fieldPush_SE_->initialize("SE_fieldPush.wav", 0.3f, false);
}

void Boss::Update() {
	if (isStart_) {
		Move();
	}

	FaceMove();
}

void Boss::Begin_Rendering(Camera3D* camera3d) {
	begin_rendering(*camera3d);
	for (uint32_t oi = 0; oi < faceParts_.size(); ++oi) {
		faceParts_[oi]->begin_rendering(*camera3d);
	}
	overLine_->begin_rendering(*camera3d);
}

void Boss::Draw() const {
	//draw();
	for (uint32_t oi = 0; oi < faceParts_.size(); ++oi) {
		faceParts_[oi]->draw();
	}

	if (isDrawOverLine_) {
		overLine_->draw();
	}
}

void Boss::Move() {
	Vector3 translate = transform->get_translate();
	translate += velocity_ * GameTimer::DeltaTime();

	translate.y -= pushBackValue_;
	pushBackValue_ -= 0.1f;
	if (pushBackValue_ <= 0.0f) {
		pushBackValue_ = 0.0f;
	}
	
	transform->set_translate(translate);
}

void Boss::FaceMove() {
	float upTranslate = faceParts_[UpperJaw_Parts]->get_transform().get_translate().z;
	float lowerTranslate = faceParts_[LowerJaw_Parts]->get_transform().get_translate().z;
	// 口を動かすアニメーションを行う
	const float step = (2.0f * PI) / static_cast<float>(movingMouth_.period);
	movingMouth_.parameter += step;
	movingMouth_.parameter = std::fmod(movingMouth_.parameter, 2.0f * PI);
	// 移動させる量を動かす
	upTranslate += std::sin(movingMouth_.parameter) * movingMouth_.amplitude;
	lowerTranslate -= std::sin(movingMouth_.parameter) * movingMouth_.amplitude;
	
	faceParts_[LowerJaw_Parts]->get_transform().set_translate_z(upTranslate);
	faceParts_[UpperJaw_Parts]->get_transform().set_translate_z(lowerTranslate);
}

void Boss::OnCollision() {
	satietyLevel_++;
	pushBackValue_ += pushBackStrength_;
	bossHit_SE_->restart();
}

bool Boss::GetIsClear() {
	if (satietyLevel_ >= satietyLevelLimit_) {
		return true;
	}

	return false;
}

bool Boss::GetIsGameOver(const float& cylinderHight) {
	if (cylinderHight < world_position().y) {
		closeStartUp_ = faceParts_[UpperJaw_Parts]->get_transform().get_translate().z;
		closeStartLow_ = faceParts_[LowerJaw_Parts]->get_transform().get_translate().z;
		return true;
	}
	return false;
}

void Boss::PlayFieldPushSE() {
	fieldPush_SE_->restart();
}

void Boss::FaceSet() {
	if (++frameCount_ <= 100) {
		isFinish_ = false;
		float t = static_cast<float>(frameCount_) / 2.0f;

		transform->set_translate_y(std::lerp(-130.0f, -2.0f, EaseOut::Expo(t)));

	} else {
		isFinish_ = true;
		frameCount_ = 0;
	}
}

void Boss::FaceShake() {
	if (++frameCount_ <= 180) {
		float angle = std::sinf(static_cast<float>(frameCount_ * 4.0f) * ToRadian) * ((PI) / 6.0f);
		Quaternion rotateValue = Quaternion::AngleAxis({ 0,1.0f,0.0f }, angle);
		Vector3 velocity = (Vector3{ 0.0f, 1.0f, 0.0f }).normalize_safe();
		float targetAngle = std::atan2f(velocity.x, velocity.z);
		Quaternion targetRotateValue = Quaternion::EulerRadian({ 0,targetAngle,0 });
		transform->set_rotate(rotateValue * targetRotateValue);
	} else {
		isFinish_ = true;
		frameCount_ = 0;
	}
}

void Boss::MouthClose() {
	float upTranslate = faceParts_[UpperJaw_Parts]->get_transform().get_translate().z;
	float lowerTranslate = faceParts_[LowerJaw_Parts]->get_transform().get_translate().z;
	float inMouthScale = faceParts_[InMouth_Parts]->get_transform().get_scale().z;
	
	// 口を閉じるアニメーションを行う
	if (++frameCount_ < 120) {
		float t = static_cast<float>(frameCount_) / 120.0f;
		// 上-6, 下1.4, 口内0.5
		upTranslate = std::lerp(closeStartUp_, -6.0f, EaseInOut::Elastic(t));
		lowerTranslate = std::lerp(closeStartLow_, 1.4f, EaseInOut::Elastic(t));
		inMouthScale = std::lerp(1.0f, 0.5f, EaseInOut::Back(t));
	} else {
		isFinish_ = true;
	}
	
	faceParts_[LowerJaw_Parts]->get_transform().set_translate_z(lowerTranslate);
	faceParts_[UpperJaw_Parts]->get_transform().set_translate_z(upTranslate);
	faceParts_[InMouth_Parts]->get_transform().set_scale({1,1,inMouthScale});
}


#ifdef _DEBUG
#include "externals/imgui/imgui.h"
void Boss::EditImGui() {
	ImGui::Begin("Boss");
	ImGui::SliderInt("satietyLevel", &satietyLevel_, 0, satietyLevelLimit_);
	ImGui::DragInt("satietyLevelLimit", &satietyLevelLimit_);
	ImGui::DragFloat("pushBackStrength", &pushBackStrength_, 0.1f, 0.0f, 1.0f);
	ImGui::DragScalar("period", ImGuiDataType_U32, &movingMouth_.period);
	ImGui::DragFloat("amplitude", &movingMouth_.amplitude, 0.1f, 0.0f, 1.0f);
	for (uint32_t oi = 0; oi < faceParts_.size(); ++oi) {
		faceParts_[oi]->debug_gui();
	}
	ImGui::Separator();
	debug_gui();
	ImGui::End();
}
#endif
