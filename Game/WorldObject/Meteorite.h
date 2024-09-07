#pragma once
#include "Engine/Game/GameObject/GameObject.h"
#include "Engine/Math/Vector3.h"

static const float kGravity_ = -9.8f;

class Meteorite
	: public GameObject {

public:

	static float attractionedStrength_;
	static float kSpeed_;
	static float radius_;

public:

	Meteorite(const Vector3& pos = Vector3(0,0,0));
	~Meteorite();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init(const Vector3& pos);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

public:	// メンバ関数

	void Move();

	void Falling();

	void OnCollision(const Vector3& other = Vector3{0,0,0});

#ifdef _DEBUG
	void EditImGui();
#endif

public:	// accessor

	// ----------- 引き寄せられているか ----------- //
	void SetIsAttraction(const bool& isAttraction) { isAttraction_ = isAttraction; }

	// ----------- 加速度 ----------- //
	void SetAcceleration(const Vector3& acceleration) { acceleration_ = acceleration; }

	// ----------- 速度 ----------- //
	const Vector3 GetVelocity() const { return velocity_; }

	// ----------- 半径 ----------- //
	const float GetRadius() const { return radius_; }

	// ----------- 死亡フラグ ----------- //
	void SetIsDead(const bool& isDead) { isDead_ = isDead; }
	const bool GetIsDead() const { return isDead_; }

private:

	Vector3 velocity_;
	Vector3 acceleration_;

	bool isFalling_;
	bool isAttraction_;	// 引き寄せられているか
	bool isDead_;


};

