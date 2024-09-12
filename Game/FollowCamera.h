#pragma once
#include <vector>
#include "Engine/Game/Camera/Camera3D.h"
#include "Game/Easing.h"
#include "Game/GameCharacter/Manager/AdjustmentItem.h"
#include "Game/Easing.h"

class Player;

class FollowCamera : public Camera3D {
public:

	struct CameraMoveDara {
		Vector3 pos;
		Vector3 rotateDegree;
		uint32_t moveTime;
	};

public:
	void initialize() override;
	void update() override;

	void SetOffset(const Vector3& offset_);
	void SetPlayer(const Player* player);

	void GameStartPerformance();

	void GameOverPerformance();

	void GameClearPerformance();

	void SetIsPerformanceFinish(const bool& isPerformanceFinish) { isPerformanceFinish_ = isPerformanceFinish; }
	const bool GetIsPerformanceFinish() const { return isPerformanceFinish_; }

	void SetIsStop(const bool& isStop) { isStop_ = isStop; }
	const bool GetIsStop() const { return isStop_; }

	const uint32_t GetNowIndex() const { return nowIndex_; }

#ifdef _DEBUG
	void Restart();
#endif // _DEBUG

private:
	Vector3 offset;

	const Player* pPlayer;

	AdjustmentItem* adjustmentItem_ = nullptr;

	std::vector<CameraMoveDara> cameraMove_;

	bool isPerformanceFinish_ = false;
	bool isStop_ = false;

	uint32_t nowIndex_ = 0;
	uint32_t pauseIndex_ = 2;

	uint32_t frameCount_ = 0;
	uint32_t frameTime_ = 200;

};
