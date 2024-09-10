#include "Meteorite.h"
#include "Engine/Game/Color/Color.h"
#include "Game/Enviroment.h"

uint32_t Meteorite::nextSerialNumber = 0;

float Meteorite::kAttractionedStrength_ = 100;
float Meteorite::kSpeed_ = -2.0f;
float Meteorite::radius_ = 1.0f;

Meteorite::Meteorite(const Vector3& pos) {
	serialNumber_ = nextSerialNumber;
	// 次の番号を加算
	++nextSerialNumber;
	Init(pos);
}

Meteorite::~Meteorite() {
	meteoHit_SE_->stop();
	meteoHit_SE_->finalize();
}

void Meteorite::Init(const Vector3& pos) {
	reset_object("particle.obj");
	Vector3 position = pos;
	transform->set_translate(pos);
	transform->set_translate_y(14.0f);
	transform->set_scale({ radius_, radius_, radius_ });

	sphereCollider_ = std::make_unique<SphereCollider>();
	sphereCollider_->initialize();
	sphereCollider_->get_hierarchy().set_parent(this->get_hierarchy());
	sphereCollider_->set_on_collision(std::bind(&Meteorite::On_Collision, this, std::placeholders::_1, &this->get_materials()[0].color));
	sphereCollider_->set_radius(1.0f);

	velocity_ = { -2, 0, 0 };

	attractionedStrength_ = kAttractionedStrength_;
	speed_ = kSpeed_;
	attractionRange_ = 3.0f;

	isDead_ = false;
	isAttraction_ = false;
	isFalling_ = false;
	isEnemyHit_ = false;

	meteoHit_SE_ = std::make_unique<AudioPlayer>();
	meteoHit_SE_->initialize("SE_meteoEachOther.wav", 0.5f, false);
	//meteoHit_SE_->initialize("SE_meteoEachOther.wav", 0.5f, false);

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
		isAttraction_ = false;
	} else {
		Falling();
	}
}

void Meteorite::Move(const Vector3& playerPosition) {
	;
	velocity_.y = 0;
	acceleration_.y = 0;
	Vector3 translate = transform->get_translate();
	// 引き寄せられている間の処理
	if (isAttraction_) {
		acceleration_ += (targetPosition_ - world_position()).normalize_safe() * kDeltaTime;
		velocity_ = (acceleration_ * attractionedStrength_) * kDeltaTime;
	}

	// 範囲外に出たら削除する処理
	if (std::abs(translate.x) > 20 || std::abs(translate.y) > 20 || std::abs(translate.z) > 20) {
		isDead_ = true;
	}

	translate += velocity_ * kDeltaTime;
	transform->set_translate(translate);
}

void Meteorite::Falling() {
	acceleration_ = { 0,kGravity_,0 };
	Vector3 translate = transform->get_translate();
	velocity_ += acceleration_ * kDeltaTime;
	translate += velocity_ * kDeltaTime;
	transform->set_translate(translate);
}

void Meteorite::On_Collision(const BaseCollider* const other, Color* object) {
	if (other->group() == "Meteo") {
		if (!isFalling_) {
			velocity_ += (other->world_position() - world_position()).normalize_safe() * -2.0f;
		}
		isFalling_ = true;
		meteoHit_SE_->play();

	} else if (other->group() == "Enemy") { // Enemy
		isEnemyHit_ = true;
		*object = { 1.0f,0,0,1.0f };
	}
}

void Meteorite::On_Collision_Enter(const BaseCollider* const) {
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