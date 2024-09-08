#pragma once
#include "Engine/Game/GameObject/GameObject.h"
#include "Engine/Math/Vector3.h"

enum class EnemyType {
	Normal_Type,
	SpecialPop_Type
};

class Enemy
	:public GameObject {

public:	

	Enemy(const Vector3& position = Vector3{0,0,0}, const EnemyType& enemyType = EnemyType::Normal_Type);
	~Enemy();

public:	// メンバ関数

	void Init(const Vector3& position, const EnemyType& enemyType);
	void Update();

	void OnCollision(const Vector3& other, const uint32_t& typeId);

public:

	const float GetRadius() const { return radius_; }

private:

	EnemyType enemyType_;

	float radius_ = 1.0f;

	Vector3 velocity_;

};

