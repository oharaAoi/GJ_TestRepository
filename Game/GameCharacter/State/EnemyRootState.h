#pragma once
#include "Game/GameCharacter/State/BaseEnemyState.h"

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

};

