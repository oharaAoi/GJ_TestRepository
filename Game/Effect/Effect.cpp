#include "Effect.h"
#include "Game/Effect/EffectManager.h"
#include "externals/imgui/imgui.h"

Effect::Effect(EffectManager* effectManager, const std::string& effectName,
			   const Vector3& centerPos, const Vector3& direction) {
	Init(effectManager, effectName, centerPos, direction);
}

Effect::~Effect() {}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Effect::Init(EffectManager* effectManager, const std::string& effectName,
				  const Vector3& centerPos, const Vector3& direction) {
	effectManager_ = effectManager;
	effectName_ = effectName;

	emitterPos_ = centerPos;
	emitterDirection_ = direction;

	LoadEffectFile(effectName);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Effect::Update() {
	for (std::list<Emitter>::iterator emitter = emitterList_.begin(); emitter != emitterList_.end();) {
		if (!emitter->GetIsDead()) {
			emitter->Update();
			emitter++;
		} else {
			emitter = emitterList_.erase(emitter);
		}
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void Effect::Draw() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　ImGuiの変更
//////////////////////////////////////////////////////////////////////////////////////////////////

void Effect::EditImGui() {
	if (ImGui::TreeNode(effectName_.c_str())) {
		for (std::list<Emitter>::iterator emitter = emitterList_.begin(); emitter != emitterList_.end();) {
			emitter->EditImGui();
			emitter++;
		}
		ImGui::TreePop();
	}
}

// ------------------- Effectの情報を読み込む ------------------- //
void Effect::LoadEffectFile(const std::string& fileName) {
	std::string filePath = kDirectoryPath_ + fileName;
	std::ifstream file(filePath);
	if (file.is_open()) {
		nlohmann::json json;
		file >> json;  // JSONファイルを読み込んでjsonオブジェクトに変換
		file.close();

		// std::list<std::string>に変換
		useEmitterNameList_.clear();
		for (const auto& name : json) {
			useEmitterNameList_.push_back(name.get<std::string>());
		}
	} else {
		assert("can not file open");
	}

	// リストには入っている名前のEmitterを読み込む
	for (const std::string& emitterName : useEmitterNameList_) {
		emitterList_.emplace_back(effectManager_, emitterName, emitterPos_, emitterDirection_);
	}
}
