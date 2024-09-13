#pragma once
#include <optional>
#include <cmath>
#include "Game/Easing.h"
#include "Engine/Game/GameObject/GameObject.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Definition.h"
#include "Game/GameCharacter/State/BaseEnemyState.h"
#include "Game/GameCharacter/State/EnemyRootState.h"
#include "Game/GameCharacter/State/EnemyApproachState.h"
#include "Game/GameCharacter/State/EnemyBlownState.h"
#include "Engine/Game/Collision/Collider/SphereCollider.h"
#include "Engine/Game/Audio/AudioPlayer.h"
#include "Engine/Game/GameTimer/GameTimer.h"
#include "Game/Effect/EffectManager.h"

class EnemyManager;

enum class EnemyType {
	Normal_Type,
	SpecialPop_Type
};

enum class EnemyState {
	Root_State,
	Approach_State,
	Blown_State,
};

enum class FieldOutMove {
	Rotate_Move,
	Stop_Move,
	Struggle_Move,
	GoField_Move
};

class Enemy
	:public GameObject {

public:

	Enemy(const Vector3& position = Vector3{ 0,0,0 }, const EnemyType& enemyType = EnemyType::Normal_Type);
	~Enemy();

public:	// メンバ関数

	void Finalize();

	void Init(const Vector3& position, const EnemyType& enemyType);
	void Update(const Vector3& playerPosition);

	void Attack();

	void ConstrainToField(Vector3& translate);

	void ChangeState(std::unique_ptr<BaseEnemyState> state);

	void CheckBehaviorRequest();

	void SetBehaviorRequest(const EnemyState& request) {
		behaviorRequest_ = request;
	}

	void On_Collision(const BaseCollider* const other);
	void On_Collision_Enter(const BaseCollider* const other);
	void On_Collision_Exit(const BaseCollider* const);

public:

	std::weak_ptr<BaseCollider> GetCollider() { return sphereCollider_; }

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

	// ----------- 攻撃中か ----------- //
	const bool GetIsAttack() const { return isAttack_; }
	void SetIsAttack(const bool& isAttack) { isAttack_ = isAttack; }

	// ----------- 生存フラグ ----------- //
	const bool GetIsDead() const { return isDead_;}
	void SetIsDead(const bool& isDead) { isDead_ = isDead; }

	// -----------  ----------- //
	void SetIsPlayerFlragPtr(bool* isBool) { isPlayerFlragPtr_ = isBool; }

	// -----------  ----------- //
	const bool GetIsKickToPlayer() const { return isKickToPlayer_; }

	// -----------  ----------- //
	const Vector3 GetRotateAxis() const { return rotateAxis_; }

private:

	EffectManager* effectManager_ = nullptr;

	std::unique_ptr<BaseEnemyState> state_ = nullptr;
	std::shared_ptr<SphereCollider> sphereCollider_ = nullptr;

	EnemyState behavior_ = EnemyState::Root_State;
	std::optional<EnemyState> behaviorRequest_ = std::nullopt;

	EnemyType enemyType_;
	float radius_ = 1.0f;

	Vector3 velocity_;
	Vector3 acceleration_;

	Vector3 playerPosition_;

	bool isAttack_ = false;
	bool isDead_ = false;
	bool* isPlayerFlragPtr_ = nullptr;
	bool isKickToPlayer_ = false;
	bool isFieldOut_ = false;

	uint32_t frameCount_;
	const uint32_t attackTime_ = 50;

	uint32_t fieldOutCount_ = 0;
	uint32_t fieldOutTime_ = 140;

	Vector3 rotateAxis_;

	FieldOutMove fieldOutMove_;

	std::unique_ptr<AudioPlayer> enemyAttack_SE_ = nullptr;
	std::unique_ptr<AudioPlayer> enemyEachOther_SE_ = nullptr;
};

