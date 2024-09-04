#pragma once
// STL
#include "stdint.h"
// Math
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Definition.h"
#include "Game/MyRandom.h"
// GameObject
#include "Game/Effect/Particle.h"

#include "Engine/GameObject/PolygonMesh/PolygonMeshManager/PolygonMeshManager.h"

class EffectManager;

/// <summary>
/// Particleを出現させるクラス
/// </summary>
class Emitter {
public:

	Emitter(EffectManager* effectManager);
	~Emitter();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init(EffectManager* effectManager);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw() const;

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////
	
	/// <summary>
	/// Particleを放出する関数
	/// </summary>
	void Emit();

	/// <summary>
	/// ImGuiで編集する関数
	/// </summary>
	void EditImGui();

	/// <summary>
	/// Emitterの情報を保存する
	/// </summary>
	void SaveFileEmitter();

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　accessor
//////////////////////////////////////////////////////////////////////////////////////////////////

	void GetEffectManager(EffectManager* effectManager) { effectManager_ = effectManager; };

private:

	EffectManager* effectManager_ = nullptr;

	// ------------------- emitterに関する変数 ------------------- //
	Vector3 centerPos_;			// emitterの中心位置
	Vector3 direction_;			// emitterがどの方向を向いているか
	Vector3 range_;				// emitterのサイズ(生成する範囲)
	
	uint32_t createTime_;		// 何フレームごとに生成するか
	uint32_t frameCreateCount_;	// 1フレームに何個生成するか
	uint32_t createCount_;		// 生成時に何個生成するか
	uint32_t lifeTime_;				// particleの生存時間

	float speed_;
	float radius_;
	float angle_;

	std::string useObjName_;

	// ------------------- particleに関する変数 ------------------- //
	Vector3 particleTranslation_;	// particleの位置
	Vector3 particleScale_;			// particleの大きさ
	Vector3 particleVelocity_;		// particleの速度

};