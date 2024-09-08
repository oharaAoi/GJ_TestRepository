#pragma once
#include "Game/GameCharacter/State/BaseEnemyState.h"
#include "Game/GameCharacter/State/EnemyApproachState.h"

/// <summary>
/// Enemyの通常時の状態
/// </summary>
class EnemyRootState :
	public BaseEnemyState {
public:

	EnemyRootState(Enemy* enemy);
	~EnemyRootState();

	void Update()override;

	void Draw() override;

private:

	uint32_t frameCount_ = 0;
	uint32_t attackTime_ = 300;
};

