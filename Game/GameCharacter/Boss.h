#pragma once
#include <cmath>
#include "Engine/Game/GameObject/GameObject.h"
#include "Game/Input/Input.h"
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Quaternion.h"
#include "Game/GameObject/GravityRod.h"
#include "Engine/Game/Camera/Camera3D.h"

class Boss 
: public GameObject {
public:

	Boss();
	~Boss();

	void Init();

	void Update();

public:	// メンバ変数



private:



};

