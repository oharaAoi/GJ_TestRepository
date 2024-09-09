#pragma once
#include "Engine/Game/GameObject/GameObject.h"
#include "Engine/Math/Vector3.h"

/// <summary>
/// playerやEnemyが上に存在しているobject
/// </summary>
class Field :
	public GameObject {
public:

	Field();
	~Field();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	void Move();

public:	// メンバ関数

	void EditImGui();

public:	// accessor

	const float GetSurfaceHight() const {return surfaceHight_;}

	// ----------- 半径 ----------- //
	const float GetRadius() const { return radius_; }

	// ----------- 座標 ----------- //
	const Vector3 GetCenterPos() const { return transform->get_translate(); }

	// ----------- 沈むか ----------- //
	void SetIsSink(const bool& isSink) { isSink_ = isSink; }
	const bool GetIsSink() const { return isSink_; }

	void SetVelocityY(const float& VelocityY) { velocityY_ = VelocityY; }

private:

	float radius_ = 5.7f;

	bool isSink_ = false;
	bool isFloatUp_ = false;

	float surfaceHight_ = 0;
	float cylinderHight_ = 0;

	float sinkValue_ = 0.3f;

	float velocityY_;
	float accelerationY_;

	float sinkLimitHight_ = 3.0f;

};