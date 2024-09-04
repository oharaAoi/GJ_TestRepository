#pragma once
// STL
#include <list>
// GameObject
#include "Game/Effect/Particle.h"
#include "Game/Effect/Emitter.h"
// Camera
#include "Engine/Game/Camera/Camera3D.h"

/// <summary>
/// 粒子を管理するクラス
/// </summary>
class EffectManager {
public:

	EffectManager();
	~EffectManager();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();
	
	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 描画前処理
	/// </summary>
	void BeginRendering(const Camera3D& camera3D);

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw() const;

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

	/// <summary>
	/// ImGuiの編集
	/// </summary>
	void EditImGui();

	/// <summary>
	/// リストにParticleを追加する
	/// </summary>
	void AddParticleList(const Vector3& translation, const Vector3& scale,
						 const Vector3& velocity, const uint32_t& lifeTime, const float& speed);

private:

	std::list<Particle> particleList_;
	std::list<Emitter> emitterList_;

	std::list<Emitter> testEmitterList_;
};

