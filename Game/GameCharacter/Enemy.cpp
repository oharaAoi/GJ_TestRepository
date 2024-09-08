#include "Enemy.h"
#include "Game/Enviroment.h"

Enemy::Enemy(const Vector3& position, const EnemyType& enemyType) { Init(position, enemyType);}
Enemy::~Enemy() {
}

void Enemy::Init(const Vector3& position, const EnemyType& enemyType) {
	reset_object("mob.obj");
	transform->set_translate(position);
	transform->set_translate_y(14.0f);
	enemyType_ = enemyType;

	isFalling_ = false;

	ChangeState(std::make_unique<EnemyRootState>(this));
}

void Enemy::Update(const Vector3& playerPosition) {
	playerPosition_ = playerPosition;
	state_->Update();

	Vector3 translate = transform->get_translate();
	translate += velocity_ * kDeltaTime;
	transform->set_translate(translate);
}

void Enemy::OnCollision(const Vector3& other, const uint32_t& typeId) {
	if (typeId == 0) {
		if (!isFalling_) {
			velocity_ = { 0,0,0 };
			velocity_ = Vector3::Normalize(other - transform->get_translate()) * -7.0f;
			acceleration_ = Vector3::Normalize(other - transform->get_translate()) * -10.0f;
			ChangeState(std::make_unique<EnemyBlownState>(this));
		}
	} else if (typeId == 1) {
		isFalling_ = true;
		velocity_ += Vector3::Normalize(other - transform->get_translate()) * -2.0f;
		ChangeState(std::make_unique<EnemyBlownState>(this));
	}
}

void Enemy::ChangeState(std::unique_ptr<BaseEnemyState> state) {
	state_ = std::move(state);
}
