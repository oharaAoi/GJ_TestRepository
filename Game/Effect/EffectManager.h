#pragma once
// STL
#include <memory>
#include <list>
#include <fstream>
#include <iostream>
// GameObject
#include "Game/Effect/Particle.h"
#include "Game/Effect/Effect.h"
// Camera
#include "Engine/Game/Camera/Camera3D.h"

class Effect;

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
	/// Emitterを作成する関数
	/// </summary>
	void CreateEmitter();

	/// <summary>
	/// ImGuiの編集
	/// </summary>
	void EditImGui();

	/// <summary>
	/// フォルダ内にあるEmitterのファイル名を取得する
	/// </summary>
	void LoadAllEmitterFaileName();

	/// <summary>
	/// フォルダ内にあるEffectのファイル名を取得する
	/// </summary>
	void LoadAllEffectFaileName();

	/// <summary>
	/// 
	/// </summary>
	void CreateEffect(const std::string& effectName);

	/// <summary>
	/// リストにParticleを追加する
	/// </summary>
	void AddParticleList(const Vector3& translation, const Vector3& scale,
						 const Vector3& velocity, const uint32_t& lifeTime, const float& speed);

private:

	const std::string kEffectDirectoryPath_ = "./Engine/Resources/Effect/";
	const std::string kEmitterDirectoryPath_ = "./Engine/Resources/Effect/Emitter/";

	// ------------------- ゲームで使用するリスト ------------------- //
	std::list<Particle> particleList_;
	std::list<Effect> effectList_;

	// ------------------- debug時に使用するリスト ------------------- //
	std::list<std::string> createEffectList_;
	std::list<Emitter> createEmitterList_;
	std::string createEmitterName_;
	char buffer[30];

	// ------------------- Effect名のリスト ------------------- //
	std::list<std::string> effectNameList_;
	std::string currentEffect_;

	// ------------------- Emitter名のリスト ------------------- //
	std::list<std::string> emitterNameList_;
	std::string currentEmitter_;
};