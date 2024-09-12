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
#include "Engine/Game/Audio/AudioPlayer.h"
#include "Engine/Game/GameTimer/GameTimer.h"


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
	void On_Collision_Enter(const BaseCollider* const other, bool* isEnemyAttack);
	void On_Collision_Exit(const BaseCollider* const);

public:	// メンバ関数

	/// <summary>
	/// 移動をする関数
	/// </summary>
	void Move();

	/// <summary>
	/// 攻撃を行う
	/// </summary>
	void Attack();

	/// <summary>
	/// 攻撃を受けた時の移動
	/// </summary>
	/// <param name="dire"></param>
	void KnockBack(const Vector3& dire);

	/// <summary>
	/// 範囲内で抑える
	/// </summary>
	void ConstrainToField(Vector3& translate);

public:

	// ----------- コライダー ----------- //
	std::weak_ptr<BaseCollider> GetCollider() { return sphereCollider_; }

	// ----------- 重力棒 ----------- //
	const GravityRod* GetGravityRod() { return gravityRod_.get(); }

	// ----------- 動く方向の回転 ----------- //
	const Quaternion& GetMoveQuaternion() const;

	// ----------- 重力の球体 ----------- //
	const Vector3 GetGravityRodVector() const { return gravityRod_->GetRodVector(); }
	const Vector3 GetGravityRodOrigine() const { return gravityRod_->GetRodOrigine(); }
	const Vector3 GetGravityRodEnd() const { return gravityRod_->GetRodEnd(); }

	// ----------- 攻撃中か ----------- //
	void SetIsAttack(const bool& isAttack) { isAttack_ = isAttack; };
	const bool& GetIsAttack() const { return isAttack_; }

	// ----------- 半径 ----------- //
	const float GetRadius() const { return radius_; }

	// ----------- 速度 ----------- //
	const Vector3 GetVelocity() const { return velocity_; }

	// ----------- 敵が攻撃中か ----------- //
	void SetIsAttackofEnmey(const bool& isAttack) { isAttackofEnmey_ = isAttack; }
	bool* GetIsAttackofEnmey() { return &isAttackofEnmey_; }

private:

	const float speed = 4.0f;

	float fieldRadius_ = 0.0f;

	std::shared_ptr<SphereCollider> sphereCollider_ = nullptr;

	Input* input_ = nullptr;
	Quaternion moveRotation;
	Quaternion forwordRotation;

	std::unique_ptr<GravityRod> gravityRod_ = nullptr;
	bool isAttack_ = false;

	// 敵が攻撃中か
	bool isAttackofEnmey_;

	Vector3 velocity_;
	Vector3 preFrameVelocity_;

	bool isStan_ = false;
	uint32_t stanFrame_ = 0;
	uint32_t stanTime_ = 60;

	float radius_ = 1.0f;

	// Arm
	std::unique_ptr<GameObject> playerArm_ = nullptr;

	std::unique_ptr<AudioPlayer> enemyKick_SE_ = nullptr;
};

