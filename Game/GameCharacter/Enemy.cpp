#include "Enemy.h"
#include "Game/Enviroment.h"
#include "Game/GameCharacter/Manager/EnemyManager.h"

Enemy::Enemy(const Vector3& position, const EnemyType& enemyType) {
	Init(position, enemyType);
}
Enemy::~Enemy() {
	Finalize();
}

void Enemy::Finalize() {
	enemyAttack_SE_->finalize();
	enemyEachOther_SE_->finalize();
}

void Enemy::Init(const Vector3& position, const EnemyType& enemyType) {
	if (enemyType == EnemyType::Normal_Type) {
		reset_object("triangleRiceBall.obj");
	} else if (enemyType == EnemyType::SpecialPop_Type) {
		reset_object("kariSpEnemy.obj");
	}

	sphereCollider_ = std::make_unique<SphereCollider>();
	sphereCollider_->initialize();
	sphereCollider_->get_hierarchy().set_parent(this->get_hierarchy());
	sphereCollider_->set_on_collision(std::bind(&Enemy::On_Collision, this, std::placeholders::_1));
	sphereCollider_->set_on_collision_enter(std::bind(&Enemy::On_Collision_Enter, this, std::placeholders::_1));

	transform->set_translate(position);
	transform->set_translate_y(13.0f);

	enemyType_ = enemyType;

	isAttack_ = false;
	isDead_ = false;
	isKickToPlayer_ = false;
	
	behaviorRequest_ = EnemyState::Root_State;

	enemyAttack_SE_ = std::make_unique<AudioPlayer>();
	enemyAttack_SE_->initialize("SE_enemyAttack.wav", 0.5f, false);
	enemyEachOther_SE_ = std::make_unique<AudioPlayer>();
	enemyEachOther_SE_->initialize("SE_enemyEachOther.wav", 0.5f, false);
}

void Enemy::Update(const Vector3& playerPosition) {
	playerPosition_ = playerPosition;

	// -------------------------------------------------
	// ↓ 攻撃中には
	// -------------------------------------------------
	if (isAttack_) {
		Attack();
		*isPlayerFlragPtr_ = true;
		return;
	}

	// -------------------------------------------------
	// ↓ 状態の確認
	// -------------------------------------------------
	CheckBehaviorRequest();

	// -------------------------------------------------
	// ↓ 状態の更新
	// -------------------------------------------------
	state_->Update();

	// -------------------------------------------------
	// ↓ 移動処理
	// -------------------------------------------------
	Vector3 translate = transform->get_translate();
	translate += velocity_ * GameTimer::DeltaTime();

	// 上限処理
	if (!isKickToPlayer_) {
		ConstrainToField(translate);
	}

	// -------------------------------------------------
	// ↓ 回転処理
	// -------------------------------------------------
	// 敵の向きを移動方向にする
	float targetAngle = std::atan2f(velocity_.x, velocity_.z);
	Quaternion moveRotate = Quaternion::EulerRadian({ 0,targetAngle,0 });
	
	// -------------------------------------------------
	// ↓ transformに送る
	// -------------------------------------------------
	translate.y = 13.0f;
	transform->set_translate(translate);
	transform->set_rotate(moveRotate);
}

void Enemy::Attack() {
	if (++frameCount_ < 30) {
		Vector3 translate = transform->get_translate();
		translate += -(playerPosition_ - translate).normalize_safe() * GameTimer::DeltaTime();
		transform->set_translate(translate);
	} else if (++frameCount_ < 50) {
		Vector3 translate = transform->get_translate();
		translate += (playerPosition_ - translate).normalize_safe() * 8.0f * GameTimer::DeltaTime();
		transform->set_translate(translate);
	} else {
		isAttack_ = false;
		velocity_ = { 0,0,0 };
		frameCount_ = 0;
		behaviorRequest_ = EnemyState::Root_State;
	}
}

void Enemy::ConstrainToField(Vector3& translate) {
	Vector3 distance = (translate - Vector3(0, translate.y, 0)).normalize_safe();
	// 中心からの長さ
	float lenght = Vector3::Length(translate, Vector3(0, translate.y, 0));
	if (lenght > 5.7f) {
		distance = distance * 5.7f;
		translate = { distance.x, translate.y, distance.z };
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
	if (other->group() == "Enemy") {
		velocity_ = { 0,0,0 };
		velocity_ = (other->world_position() - world_position()).normalize_safe() * -1.0f;
		acceleration_ = (other->world_position() - world_position()).normalize_safe() * -3.0f;
	}
}

void Enemy::On_Collision_Enter(const BaseCollider* const other) {
	// playerの攻撃されている状態かのフラグを変更する
	if (isAttack_) {
		*isPlayerFlragPtr_ = true;
	} else {
		*isPlayerFlragPtr_ = false;
	}

	if (other->group() == "Player") { // player
		if (isAttack_) {
			velocity_ *= -0.1f;
			enemyAttack_SE_->restart();
			return;
		} else {
			isKickToPlayer_ = true;
			velocity_ = (other->world_position() - world_position()).normalize_safe() * -5.0f;
			acceleration_ = (other->world_position() - world_position()).normalize_safe() * -8.0f;
			behaviorRequest_ = EnemyState::Blown_State;
		}

	} else if (other->group() == "Meteo") { // 隕石
		isDead_ = true;

	} else if (other->group() == "Enemy") { // 敵同士
		velocity_ = (other->world_position() - world_position()).normalize_safe() * -0.5f;
		acceleration_ = (other->world_position() - world_position()).normalize_safe() * -0.7f;
		behaviorRequest_ = EnemyState::Blown_State;
		enemyEachOther_SE_->restart();
	}
}

void Enemy::On_Collision_Exit(const BaseCollider* const) {

}
