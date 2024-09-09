#pragma once
#include <cmath>
#include "Engine/Game/GameObject/GameObject.h"
#include "Game/Input/Input.h"
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Quaternion.h"
#include "Game/GameObject/GravityRod.h"
#include "Engine/Game/Camera/Camera3D.h"

#include "Engine/Game/Collision/Collider/SphereCollider.h"

class Transform3D;
class Color;

class Player :
	public GameObject {

public:

	Player();
	~Player();

	void Init();

	void Update(const float& fieldRadius);

	void Begin_Rendering(Camera3D* camera3d);

	void Draw() const;

	void On_Collision(const BaseCollider* const other);
	void On_Collision_Enter(const BaseCollider* const);
	void On_Collision_Exit(const BaseCollider* const);

public:	// メンバ関数

	/// <summary>
	/// 移動をする関数
	/// </summary>
	void Move(const float& fieldRadius);

	/// <summary>
	/// 攻撃を行う
	/// </summary>
	void Attack();

public:

	std::weak_ptr<BaseCollider> GetCollider() { return sphereCollider_; }

	const GravityRod* GetGravityRod() { return gravityRod_.get(); }

	const Quaternion& GetMoveQuaternion() const;

	const Vector3 GetGravityRodVector() const { return gravityRod_->GetRodVector(); }
	const Vector3 GetGravityRodOrigine() const { return gravityRod_->GetRodOrigine(); }
	const Vector3 GetGravityRodEnd() const { return gravityRod_->GetRodEnd(); }

	const bool& GetIsAttack() const { return isAttack_; }

	const float GetRadius() const { return radius_; }

	void SetIsAttackofEnmey(const bool& isAttack) { isAttackofEnmey_ = isAttack; }

private:

	const float speed = 2.0f;

	std::shared_ptr<SphereCollider> sphereCollider_ = nullptr;

	Input* input_ = nullptr;
	Quaternion moveRotation;
	Quaternion forwordRotation;

	std::unique_ptr<GravityRod> gravityRod_ = nullptr;
	bool isAttack_ = false;

	bool isAttackofEnmey_;

	bool isStan_ = false;
	uint32_t stanFrame_ = 0;
	uint32_t stanTime_ = 360;

	float radius_ = 1.0f;
};

