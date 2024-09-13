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

	effectManager_ = EffectManager::GetInstance();

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

	fieldOutCount_ = 0;

	behaviorRequest_ = EnemyState::Root_State;

	enemyAttack_SE_ = std::make_unique<AudioPlayer>();
	enemyAttack_SE_->initialize("SE_enemyAttack.wav", 0.5f, false);
	enemyEachOther_SE_ = std::make_unique<AudioPlayer>();
	enemyEachOther_SE_->initialize("SE_enemyEachOther.wav", 0.5f, false);
}

void Enemy::Update(const Vector3& playerPosition) {
	playerPosition_ = playerPosition;
	Vector3 translate = transform->get_translate();

	// -------------------------------------------------
	// ↓ フィールド外に出ていたら
	// -------------------------------------------------
	if (isFieldOut_) {
		switch (fieldOutMove_) {
		case FieldOutMove::Rotate_Move:	// 回転
			translate += velocity_ * GameTimer::DeltaTime();
			if (++fieldOutCount_ < fieldOutTime_) {
				// 回転して飛ばされる処理
				Quaternion rotation = transform->get_quaternion();
				float t = float(fieldOutCount_) / float(fieldOutTime_);
				Quaternion rotateValue = Quaternion::AngleAxis(rotateAxis_, (1.0f - t) * (8.0f * ToRadian));
				transform->set_rotate(rotateValue * rotation);
				translate.y = 13.0f;
				transform->set_translate(translate);
			} else {
				fieldOutCount_ = 0;
				fieldOutTime_ = 120;
				fieldOutMove_ = FieldOutMove::Stop_Move;
				velocity_ = {0,0,0};
			}
			break;
		case FieldOutMove::Stop_Move: // 空中でとどまる
			if (++fieldOutCount_ < fieldOutTime_) {
				// ついでに回転を正しい方向に戻す処理
				Quaternion rotation = transform->get_quaternion();
				float t = float(fieldOutCount_) / float(fieldOutTime_);
				Vector3 velocity = (Vector3{ 0.0f, translate.y, 0.0f } - translate).normalize_safe();
				float targetAngle = std::atan2f(velocity.x, velocity.z);
				Quaternion rotateValue = Quaternion::EulerRadian({ 0,targetAngle,0 });
				transform->set_rotate(Quaternion::Slerp(rotation, rotateValue, EaseIn::Expo(t)));

				if (fieldOutTime_ - fieldOutCount_ < 10) {
					transform->set_scale({ 1.05f, 1.05f, 1.05f });
				}

			} else {
				fieldOutCount_ = 0;
				fieldOutTime_ = 180;
				fieldOutMove_ = FieldOutMove::Struggle_Move;
				transform->set_scale({ 1.0f, 1.0f, 1.0f });
				Vector3 velocity = (Vector3{ 0.0f, translate.y, 0.0f } - translate).normalize_safe();
				velocity_ = velocity;
			}
			break;
		case FieldOutMove::Struggle_Move: // ワタワタ
			if (++fieldOutCount_ < fieldOutTime_) {
				// sin波を使ってY軸中心で左右に向かせる
				float angle = std::sinf(static_cast<float>(fieldOutCount_ * 4.0f) * ToRadian) * ((PI) / 6.0f);
				Quaternion rotateValue = Quaternion::AngleAxis({ 0,1.0f,0.0f }, angle);
				velocity_ = (Vector3{ 0.0f, translate.y, 0.0f } - translate).normalize_safe();
				float targetAngle = std::atan2f(velocity_.x, velocity_.z);
				Quaternion targetRotateValue = Quaternion::EulerRadian({ 0,targetAngle,0 });
				transform->set_rotate(rotateValue * targetRotateValue);

			} else {
				isKickToPlayer_ = false;
				fieldOutCount_ = 0;
				fieldOutTime_ = 140;
				fieldOutMove_ = FieldOutMove::GoField_Move;
				velocity_ = (Vector3{ 0.0f, translate.y, 0.0f } - translate).normalize_safe();
			}
			break;
		case FieldOutMove::GoField_Move:
			++fieldOutCount_;
			// sin波を使ってY軸中心で左右に向かせる
			float angle = std::sinf(static_cast<float>(fieldOutCount_ * 8) * ToRadian) * ((PI) / 12.0f);
			Quaternion rotateValue = Quaternion::AngleAxis({ 0,1.0f,0.0f }, angle);
			float targetAngle = std::atan2f(velocity_.x, velocity_.z);
			Quaternion targetRotateValue = Quaternion::EulerRadian({ 0,targetAngle,0 });
			transform->set_rotate(rotateValue * targetRotateValue);
			// 敵を進める
			velocity_ = (Vector3{ 0.0f, translate.y, 0.0f } - translate).normalize_safe();
			translate += velocity_ * GameTimer::DeltaTime();
			// 少し小さめの円周に行くまで進む
			Vector3 distance = (translate - Vector3(0, translate.y, 0)).normalize_safe();
			// 中心からの長さ
			float lenght = Vector3::Length(translate, Vector3(0, translate.y, 0));
			if (lenght < 4.0f) {
				fieldOutCount_ = 0;
				isFieldOut_ = false;
				behaviorRequest_ = EnemyState::Root_State;

			}
			translate.y = 13.0f;
			transform->set_translate(translate);

			break;
		}

		return;
	}

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
	// ↓ 回転処理
	// -------------------------------------------------
	// 敵の向きを移動方向にする
	float targetAngle = std::atan2f(velocity_.x, velocity_.z);
	Quaternion moveRotate = Quaternion::EulerRadian({ 0,targetAngle,0 });
	transform->set_rotate(moveRotate);

	// -------------------------------------------------
	// ↓ 移動処理
	// -------------------------------------------------
	translate += velocity_ * GameTimer::DeltaTime();
	// 上限処理
	ConstrainToField(translate);

	// -------------------------------------------------
	// ↓ transformに送る
	// -------------------------------------------------
	translate.y = 13.0f;
	transform->set_translate(translate);
}

void Enemy::Attack() {
	if (++frameCount_ < 40) {
		Vector3 translate = transform->get_translate();
		translate += -(playerPosition_ - translate).normalize_safe() * GameTimer::DeltaTime();
		transform->set_translate(translate);
	} else if (++frameCount_ < 70) {
		Vector3 translate = transform->get_translate();
		translate += (playerPosition_ - translate).normalize_safe() * 10.0f * GameTimer::DeltaTime();
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

		if (isKickToPlayer_) {
			isFieldOut_ = true;
			rotateAxis_ = RandomVector3(-1, 1);
		}
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
			fieldOutMove_ = FieldOutMove::Rotate_Move;
			behaviorRequest_ = EnemyState::Blown_State;
		}

	} else if (other->group() == "Meteo") { // 隕石
		isDead_ = true;

	} else if (other->group() == "Enemy") { // 敵同士
		velocity_ = (other->world_position() - world_position()).normalize_safe() * -2.0f;
		acceleration_ = (other->world_position() - world_position()).normalize_safe() * -4.0f;
		behaviorRequest_ = EnemyState::Blown_State;

		// 場外に出ている場合でも
		if (isFieldOut_) {
			velocity_ = (other->world_position() - world_position()).normalize_safe() * -2.0f;
			acceleration_ = (other->world_position() - world_position()).normalize_safe() * -4.0f;
			behaviorRequest_ = EnemyState::Blown_State;
			fieldOutMove_ = FieldOutMove::Rotate_Move;
			fieldOutCount_ = 0;
			fieldOutTime_ = 120;
		}
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
			velocity_ *= -0.01f;	// 値を小さくしておく
			enemyAttack_SE_->restart();
			effectManager_->AddEffect("enemyHitPlayer", transform->get_translate(), velocity_);
			isAttack_ = false;
			behaviorRequest_ = EnemyState::Root_State;
			effectManager_->AddEffect("enemyHitPlayer", transform->get_translate(), { 0,1,1 });

			return;
		} else {
			isKickToPlayer_ = true;
			fieldOutMove_ = FieldOutMove::Rotate_Move;
			velocity_ = (other->world_position() - world_position()).normalize_safe() * -5.0f;
			acceleration_ = (other->world_position() - world_position()).normalize_safe() * -8.0f;
			behaviorRequest_ = EnemyState::Blown_State;
			effectManager_->AddEffect("meteo", transform->get_translate() , velocity_);
		}

	} else if (other->group() == "Meteo") { // 隕石
		isDead_ = true;
		effectManager_->AddEffect("meteoEachOther", other->get_transform().get_translate(), {0, 1, 0});

	} else if (other->group() == "Enemy") { // 敵同士
		velocity_ = (other->world_position() - world_position()).normalize_safe() * -0.5f;
		acceleration_ = (other->world_position() - world_position()).normalize_safe() * -0.7f;
		behaviorRequest_ = EnemyState::Blown_State;
		enemyEachOther_SE_->restart();

		// 場外に出ている場合でも
		if (isFieldOut_) {
			velocity_ = (other->world_position() - world_position()).normalize_safe() * -0.5f;
			acceleration_ = (other->world_position() - world_position()).normalize_safe() * -0.7f;
			behaviorRequest_ = EnemyState::Blown_State;
			fieldOutMove_ = FieldOutMove::Rotate_Move;
			fieldOutCount_ = 0;
			fieldOutTime_ = 120;
		}

		effectManager_->AddEffect("meteo", transform->get_translate(), {0.0f,1.0f,0.0f});
	}
}

void Enemy::On_Collision_Exit(const BaseCollider* const) {

}
