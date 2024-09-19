#include "Meteorite.h"
#include "Engine/Game/Color/Color.h"
#include "Game/Enviroment.h"

float Meteorite::kAttractionedStrength_ = 100;
float Meteorite::kSpeed_ = -2.0f;
float Meteorite::radius_ = 1.0f;

Meteorite::Meteorite(const Vector3& pos) {
	Init(pos);
}

Meteorite::~Meteorite() {
	meteoHit_SE_->finalize();
	meteoHitToEnemy_SE_->finalize();
	attracted_SE_->finalize();
}

void Meteorite::Init(const Vector3& pos) {
	reset_object("circleRiceBall.obj");
	Vector3 position = pos;
	transform->set_translate(pos);
	transform->set_translate_y(14.0f);
	transform->set_scale({ radius_, radius_, radius_ });

	sphereCollider_ = std::make_unique<SphereCollider>();
	sphereCollider_->initialize();
	sphereCollider_->get_hierarchy().set_parent(this->get_hierarchy());
	sphereCollider_->set_on_collision(std::bind(&Meteorite::On_Collision, this, std::placeholders::_1, &this->get_materials()[0].color));
	sphereCollider_->set_on_collision(std::bind(&Meteorite::On_Collision_Enter, this, std::placeholders::_1, &this->get_materials()[0].color));
	sphereCollider_->set_radius(1.0f);

	velocity_ = { -2.3f, 0, 0 };

	attractionedStrength_ = kAttractionedStrength_;
	speed_ = kSpeed_;
	attractionRange_ = 3.0f;

	isDead_ = false;
	isAttraction_ = false;
	isFalling_ = false;
	isEnemyHit_ = false;

	meteoHit_SE_ = std::make_unique<AudioPlayer>();
	meteoHit_SE_->initialize("SE_meteoEachOther.wav", 0.5f, false);
	meteoHitToEnemy_SE_ = std::make_unique<AudioPlayer>();
	meteoHitToEnemy_SE_->initialize("SE_enemyHitToMeteo.wav", 0.5f, false);
	attracted_SE_ = std::make_unique<AudioPlayer>();
	attracted_SE_->initialize("SE_enemyAtract.wav", 0.4f, true);

	effectManager_ = EffectManager::GetInstance();
}

void Meteorite::Update(const Vector3& playerPosition) {
	attractionedStrength_ = kAttractionedStrength_;
	attractionRange_ = 3.0f;
	if (isEnemyHit_) {
		speed_ = 0.5f;
		attractionedStrength_ = 120.0f;
	}

	if (!isFalling_) {
		Move(playerPosition);
	} else {
		Falling();
	}

	if (isAttraction_ && !isPreAttraction_) {
		attracted_SE_->restart();
	} else if(!isAttraction_ && isPreAttraction_) {
		attracted_SE_->stop();
	}

	isPreAttraction_ = isAttraction_;
}

void Meteorite::Move(const Vector3& playerPosition) {
	velocity_.y = 0;
	acceleration_.y = 0;
	Vector3 translate = transform->get_translate();
	// 引き寄せられている間の処理
	if (isAttraction_) {
		acceleration_ += (targetPosition_ - world_position()).normalize_safe() * GameTimer::DeltaTime();
		velocity_ = (acceleration_ * attractionedStrength_) * GameTimer::DeltaTime();
	}

	// 範囲外に出たら削除する処理
	if (translate.x < -25 || std::abs(translate.y) > 25 || std::abs(translate.z) > 25) {
		isDead_ = true;
	}

	translate += velocity_ * GameTimer::DeltaTime();
	transform->set_translate(translate);
}

void Meteorite::Falling() {
	acceleration_ = { 0,kGravity_,0 };
	Vector3 translate = transform->get_translate();
	velocity_ += acceleration_ * GameTimer::DeltaTime();
	translate += velocity_ * GameTimer::DeltaTime();
	transform->set_translate(translate);
}

void Meteorite::On_Collision(const BaseCollider* const other, Color* object) {
	if (other->group() == "Meteo") {
		if (!isFalling_) {
			velocity_ += (other->world_position() - world_position()).normalize_safe() * -2.0f;
		}
		isFalling_ = true;
		
	} else if (other->group() == "Enemy") { // Enemy
		isEnemyHit_ = true;
		*object = { 1.0f,0,0,1.0f };
		meteoHitToEnemy_SE_->restart();
		sphereCollider_->set_radius(sphereCollider_->get_radius() + 0.1f);
		Vector3 scale = transform->get_scale();
		scale = { scale.x + 0.2f,scale.y + 0.2f, scale.z + 0.2f };
		if (scale.x <= 1.6f) {
			transform->set_scale(scale);
		}
	}
}

void Meteorite::On_Collision_Enter(const BaseCollider* const other, Color* object) {
	if (other->group() == "Meteo") {
		if (!isFalling_) {
			velocity_ += (other->world_position() - world_position()).normalize_safe() * -2.0f;
			meteoHit_SE_->play();
			effectManager_->AddEffect("meteoEachOther", transform->get_translate(), { 0, 1, 0 });
		}
		isFalling_ = true;

	} else if (other->group() == "Enemy") { // Enemy
		isEnemyHit_ = true;
		*object = { 1.0f,0,0,1.0f };
		meteoHitToEnemy_SE_->restart();
		sphereCollider_->set_radius(sphereCollider_->get_radius() + 0.1f);
		Vector3 scale = transform->get_scale();
		scale = { scale.x + 0.2f,scale.y + 0.2f, scale.z + 0.2f };
		if (scale.x <= 1.6f) {
			transform->set_scale(scale);
		}
		effectManager_->AddEffect("meteoEachOther", transform->get_translate(), { 0, 1, 0 });
	}
}

void Meteorite::On_Collision_Exit(const BaseCollider* const) {
}

#ifdef _DEBUG
#include <externals/imgui/imgui.h>
void Meteorite::EditImGui() {
	ImGui::Begin("Field");
	debug_gui();
	ImGui::End();
}
#endif // _DEBUG