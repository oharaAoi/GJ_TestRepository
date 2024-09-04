#include "EffectManager.h"
#include "externals/imgui/imgui.h"

EffectManager::EffectManager() {
	Init();
}

EffectManager::~EffectManager() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectManager::Init() {
	emitterList_.emplace_back(this);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新処理
//////////////////////////////////////////////////////////////////////////////////////////////////

void EffectManager::Update() {
	for (std::list<Emitter>::iterator emitter = emitterList_.begin(); emitter != emitterList_.end();) {
		emitter->Update();
		emitter++;
	}

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

void EffectManager::EditImGui() {
	ImGui::Begin("EffectManager");
	
	ImGui::End();
}

void EffectManager::AddParticleList(const Vector3& translation, const Vector3& scale,
									const Vector3& velocity, const uint32_t& lifeTime, 
									const float& speed) {
	particleList_.emplace_back(translation, scale, velocity, lifeTime, speed);
}
