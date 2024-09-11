#pragma once
#include <memory>
#include <algorithm>
#include <cmath>
#include "Engine/Game/GameObject/GameObject.h"
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Quaternion.h"
#include "Engine/Math/Definition.h"
#include "Game/GameCharacter/Manager/AdjustmentItem.h"
#include "Engine/Game/Audio/AudioPlayer.h"
#include "Engine/Game/GameTimer/GameTimer.h"
#include "Game/Easing.h"


enum BossFaceParts {
	Face_Parts,
	LowerJaw_Parts,
	UpperJaw_Parts,
	InMouth_Parts,
	LeftEye_Parts,
	RightEye_Parts,
	LeftEyebrows_Parts,
	RightEyebrows_Parts,
};

class Boss
	: public GameObject {
public:

	
	struct Moving {
		float parameter;
		float amplitude;
		uint32_t period;
	};

public:

	Boss();
	~Boss();

	void Finalize();

	void Init();

	void Update();

	void Begin_Rendering(Camera3D* camera3d);

	void Draw() const;

public:	// メンバ変数

	void Move();

	void FaceMove();

	void OnCollision();

	bool GetIsClear();

	bool GetIsGameOver(const float& cylinderHight);

	void PlayFieldPushSE();

	void FaceSet();

	void FaceShake();

#ifdef _DEBUG
	void EditImGui();
#endif

public:	// accesser

	// ------------------- 満腹度 ------------------- //
	const int GetSatietyLevel() const { return satietyLevel_; }

	void SetIsStart(const bool& isStart) { isStart_ = isStart; }
	const bool GetIsStart() const { return isStart_; }

	void SetIsFinish(const bool& isFinish) { isFinish_ = isFinish; }
	const bool GetIsFinish() const { return isFinish_; }

private:

	AdjustmentItem* adjustmentItem_ = nullptr;

	bool isStart_ = false;

	Vector3 velocity_;
	float pushBackValue_;
	float pushBackStrength_ = 0.2f;

	int satietyLevel_ = 0;
	int satietyLevelLimit_ = 200;

	Moving movingMouth_;

	uint32_t frameCount_ = 0;
	bool isFinish_ = false;

	// 顔の表示用のゲームオブジェクト配列
	std::vector<std::unique_ptr<GameObject>> faceParts_;

	std::unique_ptr<GameObject> overLine_ = nullptr;

	std::unique_ptr<AudioPlayer> bossHit_SE_ = nullptr;
	std::unique_ptr<AudioPlayer> fieldPush_SE_ = nullptr;

};

