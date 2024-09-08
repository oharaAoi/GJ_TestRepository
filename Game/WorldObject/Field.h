#pragma once
#include "Engine/Game/GameObject/GameObject.h"
#include "Engine/Math/Vector3.h"

/// <summary>
/// playerやEnemyが上に存在しているobject
/// </summary>
class Field :
	public GameObject {
public:

	Field();
	~Field();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

public:	// メンバ関数

	void EditImGui();

public:	// accessor

	// ----------- 半径 ----------- //
	const float GetRadius() const { return radius_; }

	// ----------- 座標 ----------- //
	const Vector3 GetCenterPos() const { return transform->get_translate(); }

private:

	float radius_ = 5.7f;

};

