#pragma once
#include "Engine/Game/GameObject/GameObject.h"
#include "Engine/Math/Vector3.h"
#include "Game/GameCharacter/State/BaseEnemyState.h"
#include "Game/GameCharacter/State/EnemyRootState.h"
#include "Game/GameCharacter/State/EnemyApproachState.h"
#include "Game/GameCharacter/State/EnemyBlownState.h"

enum class EnemyType {
	Normal_Type,
	SpecialPop_Type
};

enum class EnemyState {
	Root_State,
	Approach_State,
	Blown_State
};

class Enemy
	:public GameObject {

public:	

	Enemy(const Vector3& position = Vector3{0,0,0}, const EnemyType& enemyType = EnemyType::Normal_Type);
	~Enemy();

public:	// メンバ関数

	void Init(const Vector3& position, const EnemyType& enemyType);
	void Update(const Vector3& playerPosition);

	void OnCollision(const Vector3& other, const uint32_t& typeId);

	void ChangeState(std::unique_ptr<BaseEnemyState> state);

public:

	// ----------- 半径 ----------- //
	const float GetRadius() const { return radius_; }

	// ----------- playerの位置 ----------- //
	const Vector3 GetPlayerPosition() const { return playerPosition_; }

	// ----------- 速度 ----------- //
	const Vector3 GetVelocity() const { return velocity_; }
	void SetVelocity(const Vector3& velocity) { velocity_ = velocity; }

	// ----------- 加速度 ----------- //
	const Vector3 GetAcceleration() const { return acceleration_; }
	void SetAcceleration(const Vector3& acceleration) { acceleration_ = acceleration; }

	// ----------- 落下しているか ----------- //
	const bool GetIsFalling() const { return isFalling_; }

private:

	std::unique_ptr<BaseEnemyState> state_ = nullptr;

	EnemyType enemyType_;
	float radius_ = 1.0f;

	Vector3 velocity_;
	Vector3 acceleration_;

	Vector3 playerPosition_;

	bool isFalling_ = false;

};

