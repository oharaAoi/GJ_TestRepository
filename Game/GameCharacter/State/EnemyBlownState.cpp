#include "EnemyBlownState.h"
#include "Game/GameCharacter/Enemy.h"
#include "Game/Enviroment.h"

EnemyBlownState::EnemyBlownState(Enemy* enemy) :
	BaseEnemyState("State Blown", enemy) {
}

EnemyBlownState::~EnemyBlownState() {
}

void EnemyBlownState::Update() {
	//if (enemy_->GetIsFalling()) {
	//	IsFalling(); 
	//} else {
	//	IsBlown();
	//}
}

void EnemyBlownState::Draw() {
}

void EnemyBlownState::IsBlown() {
	frameCount_++;
	Vector3 acceleration = enemy_->GetAcceleration();
	acceleration *= std::powf(dampingFactor_, GameTimer::DeltaTime());

	if (acceleration.length() < threshold_) {
		acceleration = Vector3{ 0,0,0 };
	}

	Vector3 velocity = enemy_->GetVelocity();
	velocity *= std::powf(dampingFactor_, GameTimer::DeltaTime());

	velocity.y = 0;
	acceleration.y = 0;

	enemy_->SetAcceleration(acceleration);
	enemy_->SetVelocity(velocity + (enemy_->GetAcceleration() * GameTimer::DeltaTime()));
}

void EnemyBlownState::IsFalling() {
	Vector3 acceleration = { 0,kGravity_,0 };
	enemy_->SetAcceleration(acceleration);
	enemy_->SetVelocity(enemy_->GetVelocity() + (enemy_->GetAcceleration() * GameTimer::DeltaTime()));
}
