#pragma once
#include <cmath>
#include "Engine/Game/GameObject/GameObject.h"
#include "Engine/Math/Quaternion.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Definition.h"

/// <summary>
/// 重力棒の先端
/// </summary>
class GravityRodTip :
	public GameObject {

public:

	static float radius_;

public:

	GravityRodTip(GameObject* gameObject);
	~GravityRodTip();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init(GameObject* gameObject);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update(const float& radius);

private:



};

