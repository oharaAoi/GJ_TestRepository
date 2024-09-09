#pragma once
#include <memory>
#include <algorithm>
#include "Engine/Game/GameObject/GameObject.h"
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Quaternion.h"

class Boss
	: public GameObject {
public:

	Boss();
	~Boss();

	void Init();

	void Update();

	void Begin_Rendering(Camera3D* camera3d);

	void Draw() const;

public:	// メンバ変数

	void Move();

	void OnCollision();

	bool GetIsClear();

	bool GetIsGameOver(const float& cylinderHight);

#ifdef _DEBUG
	void EditImGui();
#endif

public:	// accesser

	// ------------------- 満腹度 ------------------- //
	const int GetSatietyLevel() const { return satietyLevel_; }

private:

	std::unique_ptr<GameObject> debugObject_;

	Vector3 velocity_;
	float pushBackValue_;
	float pushBackStrength_ = 0.2f;

	int satietyLevel_ = 0;
	int satietyLevelLimit_ = 200;

};

