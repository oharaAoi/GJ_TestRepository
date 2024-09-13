#pragma once
#include "Engine/Game/GameObject/GameObject.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Game/GameTimer/GameTimer.h"
#include "Engine/Math/Definition.h"

class Transform3D;

class Particle :
	public GameObject {
public:

	Particle(const Vector3& pos, const Vector3& scale, const Vector3& velocity,
			 const uint32_t& lifeTime, const float& speed, const std::string& useObjName);
	~Particle();

	void Init(const Vector3& pos, const Vector3& scale, const Vector3& velocity,
			  const uint32_t& lifeTime, const float& speed, const std::string& useObjName);

	void Update();

	const bool GetIsDead() const { return isDead_; }

private:

	Vector3 velocity_;

	uint32_t lifeTime_;

	bool isDead_;

	float speed_;
};

