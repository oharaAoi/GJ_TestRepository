#include "Game/Effect/Emitter.h"
#include "Game/Effect/EffectManager.h"
#include "externals/imgui/imgui.h"

Emitter::Emitter(EffectManager* effectManager) {
	Init(effectManager);
}

Emitter::~Emitter() {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　初期化関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Emitter::Init(EffectManager* effectManager) {
	effectManager_ = effectManager;

	centerPos_ = { 0,0,0 };
	direction_ = { 0,1,0 };
	range_ = { 1,1,1 };
	particleScale_ = { 1,1,1 };

	lifeTime_ = 120;
	createTime_ = 120;
	frameCreateCount_ = 120;
	createCount_ = 3;

	speed_ = 1.0f;
	radius_ = 1.0f;
	angle_ = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　更新関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Emitter::Update() {
	EditImGui();

	if (--frameCreateCount_ <= 0) {
		Emit();
		frameCreateCount_ = createTime_;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　描画関数
//////////////////////////////////////////////////////////////////////////////////////////////////

void Emitter::Draw() const {
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// ↓　メンバ関数
//////////////////////////////////////////////////////////////////////////////////////////////////

// ------------------- particleを発射する ------------------- //
void Emitter::Emit() {
	for (uint32_t oi = 0; oi < createCount_; ++oi) {
		particleTranslation_.x = RandomFloat(-range_.x, range_.x) + centerPos_.x;
		particleTranslation_.y = RandomFloat(-range_.y, range_.y) + centerPos_.y;
		particleTranslation_.z = RandomFloat(-range_.z, range_.z) + centerPos_.z;

		particleScale_ = { radius_, radius_, radius_ };

		// 向きに対しての角度を算出
		float minAngle = -angle_ * ToRadian;
		float maxAngle = angle_ * ToRadian;
		float randomAngle = RandomFloat(minAngle, maxAngle);

		particleVelocity_ = direction_;
		particleVelocity_.normalize();

		effectManager_->AddParticleList(particleTranslation_, particleScale_, particleVelocity_, lifeTime_, speed_);
	}
}

// ------------------- ImGuiを編集する ------------------- //
void Emitter::EditImGui() {
#ifdef _DEBUG
	if (ImGui::TreeNode("Emitter")) {
		ImGui::DragScalar("frameCreateCount", ImGuiDataType_U32, &frameCreateCount_);
		ImGui::DragFloat3("centerPos", &centerPos_.x, 0.01f);
		ImGui::DragFloat3("direction", &direction_.x, 0.01f);
		ImGui::DragFloat3("range", &range_.x, 0.01f);
		ImGui::DragFloat("speed", &speed_, 0.1f);
		ImGui::DragFloat("angle", &angle_, 1.0f, -360.0f, 360.0f);
		ImGui::DragScalar("createTime", ImGuiDataType_U32, &createTime_);
		ImGui::DragScalar("createCount", ImGuiDataType_U32, &createCount_);
		ImGui::DragScalar("lifeTime", ImGuiDataType_U32, &lifeTime_);
		ImGui::TreePop();
	}

#endif // _DEBUG
}

void Emitter::SaveFileEmitter() {
}
