#include "EffectManager.h"
#include "externals/imgui/imgui.h"
#include "Game/Effect/Effect.h"

EffectManager::EffectManager() {
	Init();
}

EffectManager::~EffectManager() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectManager::Init() {
	LoadAllEmitterFaileName();
	LoadAllEffectFaileName();
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectManager::Update() {

	CreateEmitter();

	// --------------------------------
	// ↓ Effectの更新
	// --------------------------------
	for (std::list<Effect>::iterator effect = effectList_.begin(); effect != effectList_.end();) {
		effect->Update();
		effect++;
	}

	for (std::list<Emitter>::iterator emitter = createEmitterList_.begin(); emitter != createEmitterList_.end();) {
		emitter->Update();
		emitter++;
	}

	// --------------------------------
	// ↓ Particleの更新
	// --------------------------------
	for (std::list<Particle>::iterator particle = particleList_.begin(); particle != particleList_.end();) {
		particle->Update();
		particle++;
	}

	particleList_.remove_if([](const Particle& item) {
		if (item.GetIsDead()) {
			return true;
		}
		return false;
	});
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectManager::BeginRendering(const Camera3D& camera3D) {
	for (std::list<Particle>::iterator particle = particleList_.begin(); particle != particleList_.end(); ++particle) {
		particle->begin_rendering(camera3D);
	}
}

void EffectManager::Draw() const {
	for (std::list<Particle>::const_iterator particle = particleList_.begin(); particle != particleList_.end(); ++particle) {
		particle->draw();
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectManager::AddEffect(const std::string& effectName) {
	std::string path = effectName + ".json";
	//　引数のEffectが存在するか探索する
	for (const std::string& effect : effectNameList_) {
		if (effect == path) {
			effectList_.emplace_back(this, path, Vector3{0,0,0});
		}
	}
}

// ------------------- 新しいEmitterを作成する ------------------- //
void EffectManager::CreateEmitter() {
	ImGui::Begin("CreateEmitter");
	// --------------------------------
	// ↓ Emitterの編集/作成を行う
	// --------------------------------
	// std::stringからバッファにコピー
	strncpy_s(buffer, createEmitterName_.c_str(), sizeof(buffer));
	// ImGuiで文字を入力
	if (ImGui::InputText("EmitterName", buffer, IM_ARRAYSIZE(buffer))) {
		// バッファからstd::stringにコピー
		createEmitterName_ = buffer;
	}
	// 新しいEmitterを生成する
	if (ImGui::Button("CreateEmitter")) {
		createEmitterList_.emplace_back(this, createEmitterName_);
	}
	// Emitterの編集を行う
	for (std::list<Emitter>::iterator emitter = createEmitterList_.begin(); emitter != createEmitterList_.end();) {
		emitter->EditImGui();
		emitter++;
	}
	ImGui::End();
}

// ------------------- ImGuiを編集する ------------------- //
void EffectManager::EditImGui() {
	ImGui::Begin("EffectManager");
	
	ImGui::Text("CreateEffect");
	ImGui::SameLine();
	if (ImGui::Button("Reload")) {
		LoadAllEmitterFaileName();
		LoadAllEffectFaileName();
	}

	// --------------------------------
	// ↓ すべてのEmitter
	// --------------------------------
	if (ImGui::BeginCombo("SelectEmitter", currentEmitter_.c_str())) {
		for (const auto& name : emitterNameList_) {
			// 現在選択されているエフェクトかどうかを判定
			bool isSelected = (currentEmitter_ == name);
			if (ImGui::Selectable(name.c_str(), isSelected)) {
				// ユーザーが新しい項目を選択したら更新
				currentEmitter_ = name;
			}

			if (isSelected) {
				// 現在選択されている項目にフォーカスを設定
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	// 作成するListに名前を登録する
	if (ImGui::Button("AddCreateEffectList")) {
		if (currentEmitter_ != "") {
			createEffectList_.push_back(currentEmitter_);
		}
	}

	// ファイルに使用するEmitterを登録する
	if (ImGui::Button("SaveEffectList")) {
		CreateEffect("effect");
		createEffectList_.clear();
	}

	// --------------------------------
	// ↓ ゲーム上で使うEffect
	// --------------------------------
	ImGui::Separator();
	ImGui::Text("GameEffect");
	if (ImGui::BeginCombo("SelectEffect", currentEffect_.c_str())){
		for (const auto& name : effectNameList_){
			// 現在選択されているエフェクトかどうかを判定
			bool isSelected = (currentEffect_ == name);
			if (ImGui::Selectable(name.c_str(), isSelected)){
				// ユーザーが新しい項目を選択したら更新
				currentEffect_ = name;
			}

			if (isSelected) {
				// 現在選択されている項目にフォーカスを設定
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	for (std::list<Effect>::iterator effect = effectList_.begin(); effect != effectList_.end();) {
		effect->EditImGui();
		effect++;
	};
	ImGui::End();
}

// ------------------- Effectが保存されているファイル名を取得しておく ------------------- //
void EffectManager::LoadAllEffectFaileName() {
	effectNameList_.clear();
	// fileSystemを使用してファイル名を取得する
	for (const auto& entry : std::filesystem::directory_iterator(kEffectDirectoryPath_)) {
		effectNameList_.push_back(entry.path().filename().string());
	}
}

// ------------------- Emitterが保存されているファイル名を取得しておく ------------------- //
void EffectManager::LoadAllEmitterFaileName() {
	emitterNameList_.clear();
	// fileSystemを使用してファイル名を取得する
	for (const auto& entry : std::filesystem::directory_iterator(kEmitterDirectoryPath_)) {
		emitterNameList_.push_back(entry.path().filename().string());
	}
}

// ------------------- 作成するListに入っているEmitterを使用するEffectとして登録 ------------------- //
void EffectManager::CreateEffect(const std::string& effectName) {
	json root;

	for (const std::string& name : createEffectList_) {
		root.push_back(name);
	}

	// 書き込む
	// 書き込むJSONファイルのフルパス
	std::string filePath = kEffectDirectoryPath_ + effectName;
	// 書き込み用のファイルストリーム
	std::ofstream ofs;
	// ファイルを書き込みように開く
	ofs.open(filePath);

	// ファイルオープンが出来ているか
	if (ofs.fail()) {
		std::string message = "Faild open data file for write";
		assert(0);
		return;
	}

	// ファイルにjson文字列を書き込む(インデント幅4)
	ofs << std::setw(4) << root << std::endl; // rootにあるデータをjson文字列に変換してファイルへ
	// 閉じる
	ofs.close();
}


// ------------------- Particleをリストに追加する処理 ------------------- //
void EffectManager::AddParticleList(const Vector3& translation, const Vector3& scale,
									const Vector3& velocity, const uint32_t& lifeTime,
									const float& speed) {
	particleList_.emplace_back(translation, scale, velocity, lifeTime, speed);
}

