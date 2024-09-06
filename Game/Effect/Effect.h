#pragma once
// STL
#include <list>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
// Effect関連
#include "Game/Effect/Emitter.h"
#include "Game/Effect/Particle.h"

class EffectManager;

class Effect {
public:

	Effect(EffectManager* effectManager, const std::string& effectName,
		   const Vector3& centerPos, const Vector3& direction);
	~Effect();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(EffectManager* effectManager, const std::string& effectName,
			  const Vector3& centerPos, const Vector3& direction);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ImGuiの変更
	/// </summary>
	void EditImGui();

	/// <summary>
	/// EffectFileを読み込む
	/// </summary>
	void LoadEffectFile(const std::string& fileName);
	
private:

	const std::string kDirectoryPath_ = "./Engine/Resources/Effect/";

	EffectManager* effectManager_ = nullptr;
	// effect名
	std::string effectName_;
	// 使用するEmitterの名前リスト
	std::list<std::string> useEmitterNameList_;
	// emitterのリスト
	std::list<Emitter> emitterList_;

	// Emitterに関する変数
	Vector3 emitterPos_;
	Vector3 emitterDirection_;
};

