#include "Particle.h"
#include "Engine/Game/Transform3D/Transform3D.h"

Particle::Particle(const Vector3& pos, const Vector3& scale, const Vector3& velocity,
				   const uint32_t& lifeTime, const float& speed) {
	Init(pos, scale, velocity, lifeTime, speed);
}

Particle::~Particle() {
}

void Particle::Init(const Vector3& pos, const Vector3& scale, const Vector3& velocity,
					const uint32_t& lifeTime, const float& speed) {
	transform->set_translate(pos);
	transform->set_scale(scale);
	velocity_ = velocity;
	lifeTime_ = lifeTime;

	isDead_ = false;

	reset_object("particle.obj");
}

void Particle::Update() {
	Vector3 translate = transform->get_translate();
	translate += velocity_ * 0.1f;
	transform->set_translate(translate);

	if (--lifeTime_ <= 0) {
		isDead_ = true;
	}
}
