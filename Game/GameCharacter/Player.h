#pragma once
#include "Engine/Game/GameObject/GameObject.h"
#include "Game/Input/Input.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Quaternion.h"
#include "Engine/Math/Vector2.h"
#include <cmath>

class Transform3D;

class Player :
	public GameObject {

public:

	Player();
	~Player();

	void Init();

	void Update(const Vector3& planetPos, const float& planetRadius);

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// 移動をする関数
	/// </summary>
	void Move(const Vector3& planetPos, const float& planetRadius);

public:
	const Quaternion& GetMoveQuaternion() const;

private:

	Input* input_ = nullptr;
	Quaternion moveRotation;

};

