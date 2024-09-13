#include "Particle.h"
#include "Engine/Game/Transform3D/Transform3D.h"

Particle::Particle(const Vector3& pos, const Vector3& scale, const Vector3& velocity,
				   const uint32_t& lifeTime, const float& speed, const std::string& useObjName) {
	Init(pos, scale, velocity, lifeTime, speed, useObjName);
}

Particle::~Particle() {
}

void Particle::Init(const Vector3& pos, const Vector3& scale, const Vector3& velocity,
					const uint32_t& lifeTime, const float& speed, const std::string& useObjName) {
	transform->set_translate(pos);
	transform->set_scale(scale);
	velocity_ = velocity;
	lifeTime_ = lifeTime;

	speed_ = true;

	isDead_ = false;

	reset_object(useObjName);
}

void Particle::Update() {
	Vector3 translate = transform->get_translate();
	Vector3 preVelocity = velocity_;
	translate += (velocity_.normalize_safe());
	transform->set_translate(translate);
	
	if (--lifeTime_ <= 0) {
		isDead_ = true;
	}
}
