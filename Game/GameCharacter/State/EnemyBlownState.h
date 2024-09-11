#pragma once
#include <cmath>
#include "Game/GameCharacter/State/BaseEnemyState.h"
#include "Engine/Game/GameTimer/GameTimer.h"

class Enemy;

/// <summary>
/// 敵が飛ばされた後の処理
/// </summary>
class EnemyBlownState :
	public BaseEnemyState {
public:

	EnemyBlownState(Enemy* enemy);
	~EnemyBlownState();

	void Update() override;

	void Draw() override;

	void IsBlown();
	void IsFalling();

private:

	float dampingFactor_ = 0.2f;	// 減衰係数
	float threshold_ = 0.01f; // 閾値
	uint32_t frameCount_ = 0;
};

