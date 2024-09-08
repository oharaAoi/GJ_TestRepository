#include "EnemyRootState.h"
#include "Game/GameCharacter/Enemy.h"

EnemyRootState::EnemyRootState(Enemy* enemy) :
 BaseEnemyState("State Root", enemy) {
	frameCount_ = 0;
}

EnemyRootState::~EnemyRootState() {}
void EnemyRootState::Update() {
	++frameCount_;

	if (frameCount_ > attackTime_) {
		enemy_->SetBehaviorRequest(EnemyState::Approach_State);
	}
}

void EnemyRootState::Draw() {
}
