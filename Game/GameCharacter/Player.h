#pragma once
#include <cmath>
#include "Engine/Game/GameObject/GameObject.h"
#include "Game/Input/Input.h"
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Quaternion.h"
#include "Game/GameObject/GravityRod.h"
#include "Engine/Game/Camera/Camera3D.h"

class Transform3D;

class Player :
	public GameObject {

public:

	Player();
	~Player();

	void Init();

	void Update();

	void Begin_Rendering(Camera3D* camera3d);

	void Draw() const;

public:	// メンバ関数

	/// <summary>
	/// 移動をする関数
	/// </summary>
	void Move();

	/// <summary>
	/// 攻撃を行う
	/// </summary>
	void Attack();

public:

	const GravityRod* GetGravityRod() { return gravityRod_.get(); }

	const Quaternion& GetMoveQuaternion() const;

	const Vector3 GetGravityRodVector() const { return gravityRod_->GetRodVector(); }
	const Vector3 GetGravityRodOrigine() const { return gravityRod_->GetRodOrigine(); }
	const Vector3 GetGravityRodEnd() const { return gravityRod_->GetRodEnd(); }

	const bool& GetIsAttack() const { return isAttack_; }

private:

	const float speed = 2.0f;

	Input* input_ = nullptr;
	Quaternion moveRotation;
	Quaternion forwordRotation;

	std::unique_ptr<GravityRod> gravityRod_ = nullptr;
	bool isAttack_ = false;

};

