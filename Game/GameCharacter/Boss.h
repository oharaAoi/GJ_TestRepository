#pragma once
#include <memory>
#include <algorithm>
#include "Engine/Game/GameObject/GameObject.h"
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Quaternion.h"

class Boss 
: public GameObject {
public:

	Boss();
	~Boss();

	void Init();

	void Update();

	void Begin_Rendering(Camera3D* camera3d);

	void Draw() const;

public:	// メンバ変数

	void Move();

	void OnCollision();

#ifdef _DEBUG
	void EditImGui();
#endif

private:

	Vector3 velocity_;
	float pushBackValue_;
	float pushBackStrength_ = 0.3f;

	std::unique_ptr<GameObject> debugObject_;

};

