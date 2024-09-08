#include "EnemyApproachState.h"
#include "Game/GameCharacter/Enemy.h"

EnemyApproachState::EnemyApproachState(Enemy* enemy) :
 BaseEnemyState("State Approach", enemy) {
}
EnemyApproachState::~EnemyApproachState() {}

void EnemyApproachState::Update() {
	// playerを追いかけていく処理を行う
	Vector3 velocity = Vector3::Normalize(enemy_->GetPlayerPosition() - enemy_->get_transform().get_translate()) * kSpeed_;
	enemy_->SetVelocity(velocity);
}

void EnemyApproachState::Draw() {

}
