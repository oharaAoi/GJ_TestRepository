#pragma once
#include "Engine/Game/GameObject/GameObject.h"
#include "Engine/Math/Vector3.h"

#include "Game/MyRandom.h"

class Meteorite
	: public GameObject {

public:

	Meteorite();
	~Meteorite();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

public:	// メンバ関数

	void Move();

	void EditImGui();

public:	// accessor


private:

	const float speed = 2.0f;

	float radius_ = 2.0f;
	Vector3 velocity_;

};

