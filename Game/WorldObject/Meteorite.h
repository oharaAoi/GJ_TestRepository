#pragma once
#include "Engine/Game/GameObject/GameObject.h"
#include "Engine/Math/Vector3.h"

#include "Game/MyRandom.h"

class Meteorite
	: public GameObject {

public:

	static float attractionedStrength_;
	static float kSpeed_;
	static float radius_;

public:

	Meteorite();
	~Meteorite();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

public:	// メンバ関数

	void Move();

	void EditImGui();

public:	// accessor

	// ----------- 引き寄せられているか ----------- //
	void SetIsAttraction(const bool& isAttraction) { isAttraction_ = isAttraction; }

	// ----------- 加速度 ----------- //
	void SetAcceleration(const Vector3& acceleration) { acceleration_ = acceleration; }

	// ----------- 半径 ----------- //
	const float GetRadius() const { return radius_; }

	// ----------- 死亡フラグ ----------- //
	void SetIsDead(const bool& isDead) { isDead_ = isDead; }
	const bool GetIsDead() const { return isDead_; }

private:

	Vector3 velocity_;
	Vector3 acceleration_;

	bool isAttraction_;
	bool isDead_;
};

