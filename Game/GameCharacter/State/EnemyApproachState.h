#pragma once
#include "Game/GameCharacter/State/BaseEnemyState.h"

class Enemy;

/// <summary>
/// Enemyが追いかけてくる時の処理
/// </summary>
class EnemyApproachState :
	public BaseEnemyState {
public:

	EnemyApproachState(Enemy* enemy);
	~EnemyApproachState();

	void Update()override;

	void Draw() override;

private:

	float kSpeed_ = 0.3f;

	float playerToLenght_ = 0;

};

