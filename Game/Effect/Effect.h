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

	Effect(EffectManager* effectManager, const std::string& effectName);
	~Effect();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(EffectManager* effectManager, const std::string& effectName);

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

	EffectManager* effectManager_ = nullptr;
	std::string effectName_;

	std::list<std::string> useEmitterNameList_;
	std::list<Emitter> emitterList_;
};

