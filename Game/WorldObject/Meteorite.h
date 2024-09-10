#pragma once
#include "Engine/Game/GameObject/GameObject.h"
#include "Engine/Game/Collision/Collider/SphereCollider.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Game/Managers/AudioManager/AudioManager.h"
#include "Engine/Game/Audio/AudioPlayer.h"
#include "Engine/Game/Audio/AudioResource.h"

class Color;

class Meteorite
	: public GameObject {

public:

	static float kAttractionedStrength_;
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
	void Update(const Vector3& playerPosition);

public:	// メンバ関数

	void Move(const Vector3& playerPosition);

	void Falling();

	void On_Collision(const BaseCollider* const other, Color* object);
	void On_Collision_Enter(const BaseCollider* const);
	void On_Collision_Exit(const BaseCollider* const);

#ifdef _DEBUG
	void EditImGui();
#endif

public:	// accessor

	std::weak_ptr<BaseCollider> GetCollider() { return sphereCollider_; }

	const std::string GetMeteoId() const { return "Meteo" + std::to_string(serialNumber_); }

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

	// ----------- 落下フラグ ----------- //
	const bool GetIsFalling() const { return isFalling_; }

	// -----------  ----------- //
	void SetIsEnemyHit(const bool& ishit) { isEnemyHit_ = ishit; }

	void SetTargetPosition(const Vector3& targetPosition) { targetPosition_ = targetPosition; }

	const float GetAttractRange() const { return attractionRange_; }

	void SetNextCollision(const uint32_t& num) { nextCollisionType_ = num; }

private:

	uint32_t serialNumber_ = 0;
	static uint32_t nextSerialNumber;

	std::shared_ptr<SphereCollider> sphereCollider_ = nullptr;

	Vector3 targetPosition_;

	Vector3 velocity_;
	Vector3 acceleration_;

	float speed_;
	float attractionedStrength_;
	float attractionRange_;

	bool isFalling_;
	bool isAttraction_;	// 引き寄せられているか
	bool isDead_;

	bool isEnemyHit_;

	uint32_t nextCollisionType_;

	std::unique_ptr<AudioPlayer> meteoHit_SE_ = nullptr;
};

