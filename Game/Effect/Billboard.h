#pragma once
#include "Engine/Game/GameObject/GameObject.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Game/GameTimer/GameTimer.h"
#include "Engine/Math/Definition.h"
#include "Game/MyRandom.h"

class Billboard :
	public GameObject {
public:

	Billboard(const Vector3& pos);
	~Billboard();

	void Init(const Vector3& pos);

	void Update(const Vector3& camera3DTranslate);

	void Begine_Rendering(const Camera3D& camera3D);

	void Draw();

public:

	const bool GetIsDead() const { return isDead_; }

private:
	Vector3 velocity_;
	Vector3 accelaration_;
	float lifeTime_;
	bool isDead_;
};

