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
	velocity_ = velocity.normalize_safe();
	lifeTime_ = lifeTime;

	speed_ = 0.1f;

	isDead_ = false;

	axisVector_ = RandomVector3(-1, 1);

	reset_object(useObjName);

	transform->set_scale(RandomVector3(0.5f, 0.8f));
}

void Particle::Update() {
	Vector3 translate = transform->get_translate();
	translate += velocity_ * speed_;
	transform->set_translate(translate);

	Quaternion rotation = transform->get_quaternion();
	Quaternion rotateValue = Quaternion::AngleAxis(axisVector_, (8.0f * ToRadian));
	transform->set_rotate(rotateValue * rotation);
	
	if (--lifeTime_ <= 0) {
		isDead_ = true;
	}
}
