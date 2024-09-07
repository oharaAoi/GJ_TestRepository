#include "Enemy.h"

Enemy::Enemy(const Vector3& position, const EnemyType& enemyType) { Init(position, enemyType);}
Enemy::~Enemy() {
}

void Enemy::Init(const Vector3& position, const EnemyType& enemyType) {
	reset_object("mob.obj");
	transform->set_translate(position);
	enemyType_ = enemyType;
}

void Enemy::Update() {

}
