#include "Enemy.h"
#include "Game/Enviroment.h"

uint32_t Enemy::nextSerialNumber = 0;

Enemy::Enemy(const Vector3& position, const EnemyType& enemyType) {
	serialNumber_ = nextSerialNumber;
	// 次の番号を加算
	++nextSerialNumber;
	Init(position, enemyType); 
}
Enemy::~Enemy() {
}

void Enemy::Init(const Vector3& position, const EnemyType& enemyType) {
	if (enemyType == EnemyType::Normal_Type) {
		reset_object("kariEnemy.obj");
	} else if (enemyType == EnemyType::SpecialPop_Type) {
		reset_object("kariSpEnemy.obj");
	}

	sphereCollider_ = std::make_unique<SphereCollider>();
	sphereCollider_->initialize();
	sphereCollider_->get_hierarchy().set_parent(this->get_hierarchy());
	sphereCollider_->set_on_collision(std::bind(&Enemy::On_Collision, this, std::placeholders::_1));

	transform->set_translate(position);
	transform->set_translate_y(13.0f);
	enemyType_ = enemyType;

	isFalling_ = false;

	behaviorRequest_ = EnemyState::Root_State;
}

void Enemy::Update(const Vector3& playerPosition) {
	playerPosition_ = playerPosition;

	if (isAttack_) {
		Attack();
		return;
	}

	CheckBehaviorRequest();

	state_->Update();

	Vector3 translate = transform->get_translate();
	translate += velocity_ * kDeltaTime;
	transform->set_translate(translate);

	// 敵の向きを移動方向にする
	if (!isFalling_) {
		float targetAngle = std::atan2f(velocity_.x, velocity_.z);
		Quaternion moveRotate = Quaternion::EulerRadian({ 0,targetAngle,0 });
		transform->set_rotate(moveRotate);
	}

	transform->set_translate_y(13.0f);
}

void Enemy::Attack() {
	if (++frameCount_ < 30) {
		Vector3 translate = transform->get_translate();
		translate += -(playerPosition_ - translate).normalize_safe() * kDeltaTime;
		transform->set_translate(translate);
	} else if(++frameCount_ < 50) {
		Vector3 translate = transform->get_translate();
		translate += (playerPosition_ - translate).normalize_safe() * 8.0f * kDeltaTime;
		transform->set_translate(translate);
	} else {
		isAttack_ = false;
		velocity_ = { 0,0,0 };
		frameCount_ = 0;
		behaviorRequest_ = EnemyState::Root_State;
	}
}

void Enemy::OnCollision(const Vector3& other, const uint32_t& typeId) {
	if (typeId == 0) {
		if (!isFalling_) {
			velocity_ = { 0,0,0 };
			velocity_ = (other - transform->get_translate()).normalize_safe() * -7.0f;
			acceleration_ = (other - transform->get_translate()).normalize_safe() * -10.0f;
			behaviorRequest_ = EnemyState::Blown_State;
		}
	} else if (typeId == 1) {
		isFalling_ = true;
		velocity_ += (other - transform->get_translate()).normalize_safe() * -2.0f;
		behaviorRequest_ = EnemyState::Blown_State;

	} else if (typeId == 2) {
		velocity_ = { 0,0,0 };
		velocity_ = (other - transform->get_translate()).normalize_safe() * -1.0f;
		acceleration_ = (other - transform->get_translate()).normalize_safe() * -3.0f;
		behaviorRequest_ = EnemyState::Blown_State;
	} 
}

void Enemy::ChangeState(std::unique_ptr<BaseEnemyState> state) {
	state_ = std::move(state);
}

void Enemy::CheckBehaviorRequest() {
	// リクエストがあったら
	if (behaviorRequest_) {
		// 振る舞いを変更する
		behavior_ = behaviorRequest_.value();

		switch (behavior_) {
		case EnemyState::Root_State:
			ChangeState(std::make_unique<EnemyRootState>(this));
			break;
		case EnemyState::Approach_State:
			ChangeState(std::make_unique<EnemyApproachState>(this));
			break;
		case EnemyState::Blown_State:
			ChangeState(std::make_unique<EnemyBlownState>(this));
			break;
		}
		// ふるまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}
}

void Enemy::On_Collision(const BaseCollider* const other) {
	if (nextCollisionType_ == 0) { // player
		if (isAttack_) {
			velocity_ *= -1.0f;
			return;
		}
		
		if (!isFalling_) {
			velocity_ = { 0,0,0 };
			velocity_ = (other->world_position() - world_position()).normalize_safe() * -7.0f;
			acceleration_ = (other->world_position() - world_position()).normalize_safe() * -10.0f;
			behaviorRequest_ = EnemyState::Blown_State;
		}
	} else if(nextCollisionType_ == 1){ // 隕石
		isDead_ = true;
	} else if (nextCollisionType_ == 2) {// 敵同士
		velocity_ = { 0,0,0 };
		velocity_ = (other->world_position() - world_position()).normalize_safe() * -1.0f;
		acceleration_ = (other->world_position() - world_position()).normalize_safe() * -3.0f;
		behaviorRequest_ = EnemyState::Blown_State;
	}
}

void Enemy::On_Collision_Enter(const BaseCollider* const) {
}

void Enemy::On_Collision_Exit(const BaseCollider* const) {

}
