#include "Enemy.h"
#include "Game/Enviroment.h"

Enemy::Enemy(const Vector3& position, const EnemyType& enemyType) { Init(position, enemyType); }
Enemy::~Enemy() {
}

void Enemy::Init(const Vector3& position, const EnemyType& enemyType) {
	if (enemyType == EnemyType::Normal_Type) {
		reset_object("kariEnemy.obj");
	} else if (enemyType == EnemyType::SpecialPop_Type) {
		reset_object("kariSpEnemy.obj");
	}
	transform->set_translate(position);
	transform->set_translate_y(13.0f);
	enemyType_ = enemyType;

	isFalling_ = false;

	behaviorRequest_ = EnemyState::Approach_State;
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
		translate += -Vector3::Normalize(playerPosition_ - translate) * kDeltaTime;
		transform->set_translate(translate);
	} else if(++frameCount_ < 50) {
		Vector3 translate = transform->get_translate();
		translate += Vector3::Normalize(playerPosition_ - translate) * 8.0f * kDeltaTime;
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
			velocity_ = Vector3::Normalize(other - transform->get_translate()) * -7.0f;
			acceleration_ = Vector3::Normalize(other - transform->get_translate()) * -10.0f;
			behaviorRequest_ = EnemyState::Blown_State;
		}
	} else if (typeId == 1) {
		isFalling_ = true;
		velocity_ += Vector3::Normalize(other - transform->get_translate()) * -2.0f;
		behaviorRequest_ = EnemyState::Blown_State;

	} else if (typeId == 2) {
		velocity_ = { 0,0,0 };
		velocity_ = Vector3::Normalize(other - transform->get_translate()) * -1.0f;
		acceleration_ = Vector3::Normalize(other - transform->get_translate()) * -3.0f;
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
