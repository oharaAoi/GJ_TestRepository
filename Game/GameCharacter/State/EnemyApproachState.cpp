#include "EnemyApproachState.h"
#include "Game/GameCharacter/Enemy.h"

EnemyApproachState::EnemyApproachState(Enemy* enemy) :
 BaseEnemyState("State Approach", enemy) {
	playerToLenght_ = 0;
}
EnemyApproachState::~EnemyApproachState() {}

void EnemyApproachState::Update() {
	// playerを追いかけていく処理を行う
	Vector3 velocity = Vector3::Normalize(enemy_->GetPlayerPosition() - enemy_->get_transform().get_translate()) * kSpeed_;
	enemy_->SetVelocity(velocity);

	playerToLenght_ = Vector3::Length(enemy_->GetPlayerPosition(), enemy_->get_transform().get_translate());

	if (playerToLenght_ < enemy_->GetRadius() * 3.0f) {
		enemy_->SetIsAttack(true);
	}
}

void EnemyApproachState::Draw() {

}
