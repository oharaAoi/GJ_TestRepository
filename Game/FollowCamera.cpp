#include "FollowCamera.h"

#include "Game/GameCharacter/Player.h"

void FollowCamera::finalize() {
	boss_Apparance_SE_->finalize();
}

void FollowCamera::initialize() {
	Camera3D::initialize();
	adjustmentItem_ = AdjustmentItem::GetInstance();
	const char* groupName = "GameStartCameraMove";
	CameraMoveDara data{
		{0,0,0},
		{0,0,0},
		0
	};
	adjustmentItem_->AddItem(groupName, "pos_0", data.pos);
	adjustmentItem_->AddItem(groupName, "rotate_0", data.rotateDegree);
	adjustmentItem_->AddItem(groupName, "moveTime_0", data.moveTime);
	adjustmentItem_->AddItem(groupName, "pos_1", data.pos);
	adjustmentItem_->AddItem(groupName, "rotate_1", data.rotateDegree);
	adjustmentItem_->AddItem(groupName, "moveTime_1", data.moveTime);
	adjustmentItem_->AddItem(groupName, "pos_2", data.pos);
	adjustmentItem_->AddItem(groupName, "rotate_2", data.rotateDegree);
	adjustmentItem_->AddItem(groupName, "moveTime_2", data.moveTime);
	adjustmentItem_->AddItem(groupName, "pos_3", data.pos);
	adjustmentItem_->AddItem(groupName, "rotate_3", data.rotateDegree);
	adjustmentItem_->AddItem(groupName, "moveTime_3", data.moveTime);
	adjustmentItem_->AddItem(groupName, "pos_4", data.pos);
	adjustmentItem_->AddItem(groupName, "rotate_4", data.rotateDegree);
	adjustmentItem_->AddItem(groupName, "moveTime_4", data.moveTime);

	for (uint32_t oi = 0; oi < 5; ++oi) {
		std::string itemNamePos = "pos_" + std::to_string(oi);
		std::string itemNameRotate = "rotate_" + std::to_string(oi);
		std::string itemNameTime = "moveTime_" + std::to_string(oi);
		CameraMoveDara data{
		adjustmentItem_->GetValue<Vector3>(groupName, itemNamePos),
		adjustmentItem_->GetValue<Vector3>(groupName, itemNameRotate),
		adjustmentItem_->GetValue<float>(groupName, itemNameTime),
		};
		cameraMove_.push_back(data);
	}

	nowIndex_ = 0;
	pauseIndex_ = 2;

	isPerformanceFinish_ = false;
	isStop_ = false;

	boss_Apparance_SE_ = std::make_unique<AudioPlayer>();
	boss_Apparance_SE_->initialize("SE_bossAppearace.wav", 0.5f, false);
}

void FollowCamera::update() {
	
}

void FollowCamera::SetOffset(const Vector3& offset_) {
	offset = offset_;
}

void FollowCamera::SetPlayer(const Player* player) {
	pPlayer = player;
}

void FollowCamera::GameStartPerformance() {
	if (isStop_) {
		return;
	}

	if (parametric < 1.0f) {
		animationTimer += GameTimer::DeltaTime();
		parametric = (std::min)(1.0f, animationTimer / cameraMove_[nowIndex_].moveTime);
		transform->set_translate(Vector3::Lerp(cameraMove_[nowIndex_].pos, cameraMove_[nowIndex_ + 1].pos, EaseIn::Sine(parametric)));
		transform->set_rotate(Quaternion::Slerp(Quaternion::EulerDegree(cameraMove_[nowIndex_].rotateDegree),
												Quaternion::EulerDegree(cameraMove_[nowIndex_ + 1].rotateDegree), EaseIn::Sine(parametric)));
	} else {
		nowIndex_++;
		animationTimer = 0;
		parametric = 0;

		if (nowIndex_ == pauseIndex_) {
			isStop_ = true;
			boss_Apparance_SE_->play();
		}

		if (nowIndex_ >= cameraMove_.size() - 1) {
			isPerformanceFinish_ = true;
		}
	}
}

void FollowCamera::GameOverPerformance() {
	if (parametric < 1.0f) {
		isPerformanceFinish_ = false;
		animationTimer += GameTimer::DeltaTime();
		parametric = (std::min)(1.0f, animationTimer / cameraMove_[nowIndex_].moveTime);
		transform->set_translate(Vector3::Lerp(cameraMove_[4].pos, Vector3{ 0, 80, -40 }, EaseOut::Quint(parametric)));
	} else {
		isPerformanceFinish_ = true;
		animationTimer = 0;
		parametric = 0;
	}
}

void FollowCamera::GameClearPerformance() {
	if (parametric < 1.0f) {
		isPerformanceFinish_ = false;
		animationTimer += GameTimer::DeltaTime();
		parametric = (std::min)(1.0f, animationTimer / cameraMove_[nowIndex_].moveTime);
		// cameraMove_[4].posは開始時の最後の位置
		transform->set_translate(Vector3::Lerp(cameraMove_[4].pos, Vector3{ 0, -18, 14 }, EaseOut::Quint(parametric)));
		transform->set_rotate(Quaternion::Slerp(Quaternion::EulerDegree(cameraMove_[4].rotateDegree),
												Quaternion::EulerDegree({90, 0,0 }), EaseOut::Quint(parametric)));
	} else {
		isPerformanceFinish_ = true;
		parametric = 0;
	}
}

#ifdef _DEBUG
void FollowCamera::Restart() {
	nowIndex_ = 0;
	animationTimer = 0;
	parametric = 0;
	isPerformanceFinish_ = false;
	isStop_ = false;
}
#endif // _DEBUG