#pragma once
#include "Engine/Game/GameObject/GameObject.h"
#include "Engine/Game/Camera/Camera3D.h"
#include "Engine/Math/Vector3.h"
#include "GravityRodTip.h"

enum Tips {
	Tips_origine,
	Tips_end
};


/// <summary>
/// 重力を持った棒
/// </summary>
class GravityRod :
	public GameObject {
public:

	GravityRod(GameObject* gameObject);
	~GravityRod();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init(GameObject* gameObject);

	/// <summary>
	/// 描画関数
	/// </summary>
	void Update();

	void Begin_Rendering(Camera3D* camera3d);

	void Draw() const;

public:	// メンバ関数

	void EditImGui();

public:	// accessor

	// ----------- 棒のベクトル ----------- //
	const Vector3 GetRodVector() const { return rodVector_; }

	// ----------- 両先端の座標 ----------- //
	const Vector3 GetRodOrigine() const { return tipObject_[1]->world_position(); }
	const Vector3 GetRodEnd() const { return tipObject_[0]->world_position(); }

	const float GetAttractionRange() const { return attractionRange_; }

private:

	float radius_ = 4.8f;
	float attractionRange_ = 3.0f;

	std::unique_ptr<GravityRodTip> tipObject_[2];

	Vector3 rodVector_;
};

