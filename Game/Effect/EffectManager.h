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

	EffectManager() = default;
	~EffectManager();
	EffectManager(const EffectManager&) = delete;
	const EffectManager& operator=(const EffectManager&) = delete;

	static EffectManager* GetInstance();

public:

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

public:	// メンバ関数

	/// <summary>
	/// Effectを追加する
	/// </summary>
	/// <param name="effectName"></param>
	void AddEffect(const std::string& effectName,
				   const Vector3& pos, const Vector3& direction);

#ifdef _DEBUG
	/// <summary>
	/// Emitterを作成する関数
	/// </summary>
	void CreateEmitter();

	/// <summary>
	/// ImGuiの編集
	/// </summary>
	void EditImGui();
#endif

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
						 const Vector3& velocity, const uint32_t& lifeTime,
						 const float& speed, const std::string& useObjName);

private:

	const std::string kEffectDirectoryPath_ = "./Game/Resources/Effect/";
	const std::string kEmitterDirectoryPath_ = "./Game/Resources/Effect/Emitter/";

	// ------------------- ゲームで使用するリスト ------------------- //
	std::list<Particle> particleList_;
	std::list<Effect> effectList_;

	// ------------------- debug時に使用するリスト ------------------- //
	std::list<std::string> createEffectList_;
	std::list<Emitter> createEmitterList_;
	std::string createEmitterName_;
	char buffer[30];

	// ------------------- debug時に使用するEmitter ------------------- //
	std::string createEffectName_;
	char effectBuffer[30];
	std::unique_ptr<Emitter> editEmitter_ = nullptr;

	// ------------------- Effect名のリスト ------------------- //
	std::list<std::string> effectNameList_;
	std::list<Effect> editEffectList_;
	std::string currentEffect_;

	// ------------------- Emitter名のリスト ------------------- //
	std::list<std::string> emitterNameList_;
	std::string currentEmitter_;
};