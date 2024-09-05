#pragma once
#include "Engine/Game/GameObject/GameObject.h"
#include "Engine/Game/Camera/Camera3D.h"
#include "Engine/Math/Vector3.h"
#include "GravityRodTip.h"

/// <summary>
/// 重力を持った棒
/// </summary>
class GravityRod :
	public GameObject {
public:

	GravityRod();
	~GravityRod();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 描画関数
	/// </summary>
	void Update(const Vector3& playerPos, const Quaternion& quaternion);

	void Begin_Rendering(Camera3D* camera3d);

	void Draw() const;

public:	// メンバ関数

	void EditImGui();

public:	// accessor



private:

	float radius_ = 2.0f;

	std::unique_ptr<GravityRodTip> tipObject_[2];

};

