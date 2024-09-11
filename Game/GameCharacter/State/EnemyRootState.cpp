#include "EnemyRootState.h"
#include "Game/GameCharacter/Enemy.h"

EnemyRootState::EnemyRootState(Enemy* enemy) :
 BaseEnemyState("State Root", enemy) {
	frameCount_ = 0;
}

EnemyRootState::~EnemyRootState() {}
void EnemyRootState::Update() {
	RandomMove();

	if (frameCount_ >= attackTime_) {
		enemy_->SetBehaviorRequest(EnemyState::Approach_State);
	}

	++frameCount_;
}

void EnemyRootState::Draw() {
}

void EnemyRootState::RandomMove() {
	if (frameCount_ % 200 == 0) {
		Vector3 velocity = RandomVector3(-1, 1);
		enemy_->SetVelocity(velocity.normalize_safe());
	}
}
