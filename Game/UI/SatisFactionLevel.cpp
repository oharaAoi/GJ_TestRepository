#include "SatisFactionLevel.h"

SatisFactionLevel::SatisFactionLevel() {
	Init();
}

SatisFactionLevel::~SatisFactionLevel() {
}

void SatisFactionLevel::Init() {
	satisFaction_ = std::make_unique<GameObject>();
	percent_ = std::make_unique<GameObject>();
	satisFaction_->reset_object("fullStomach.obj");
	percent_->reset_object("fullStomachPercent.obj");

	satisFaction_->get_transform().set_scale({ 0.7f, 0.7f, 0.7f });
	satisFaction_->get_transform().set_translate({ -12.0f, 16.0f, -13.0f });
	percent_->get_transform().set_translate({ -2.5f, 16.0f, -13.0f });
}

void SatisFactionLevel::Update(const int& satisFaction, const int& satisFactionLevel) {
	raito_ = float(satisFaction) / float(satisFactionLevel);

	if (raito_ != 0) {
		digit_ = static_cast<int>(std::log10f(static_cast<float>(raito_ * 100.0f)) + 1);
		if (digit_ != score_.size()) {
			DigitCreate();
		}
	}

	ScoreChange();
}

void SatisFactionLevel::BeginRendering(Camera3D* camera3d) {
	satisFaction_->begin_rendering(*camera3d);
	percent_->begin_rendering(*camera3d);

	for (uint32_t oi = 0; oi < score_.size(); ++oi) {
		score_[oi]->begin_rendering(*camera3d);
	}
}

void SatisFactionLevel::Draw() {
	satisFaction_->draw();
	percent_->draw();
	for (uint32_t oi = 0; oi < score_.size(); ++oi) {
		score_[oi]->draw();
	}
}

void SatisFactionLevel::ScoreChange() {
	// 最大の桁数を求める
	int nowDigite = static_cast<int>(std::powf(10.0f, static_cast<float>(digit_)));
	nowDigite /= 10;
	int num = 0;

	for (int oi = static_cast<int>(score_.size()) - 1; oi >= 0; oi--) {
		// 頭からスコアの数値を取り出し10で割ったあまりから数値割り出す(123で2桁目を取り出したい時12にして10で割る)
		num = (int(raito_ * 100.0f) / nowDigite) % 10;

		// 描画する画像を切り替える
		NumberChange(num, oi);

		// 割る桁を減らし取り出したい位置を右にずらす
		nowDigite /= 10;
	}
}

void SatisFactionLevel::DigitCreate() {
	int num = digit_ - static_cast<int>(score_.size());
	while (num != 0) {
		// 初期化した値を実際に描画する変数に入れる
		auto& newScore = score_.emplace_back(std::make_unique<GameObject>());
		newScore->get_transform().set_translate({ -2.0f * (2.5f + float(score_.size() - 1)), 16.0f, -13.0f
	});
		num--;
	}
}

void SatisFactionLevel::NumberChange(const int& number, const int& index) {
	if (number == 0) {
		score_[index]->reset_object("fullStomach0.obj");
	}
	else if (number == 1) {
		score_[index]->reset_object("fullStomach1.obj");
	} else if (number == 2) {
		score_[index]->reset_object("fullStomach2.obj");
	} else if (number == 3) {
		score_[index]->reset_object("fullStomach3.obj");
	} else if (number == 4) {
		score_[index]->reset_object("fullStomach4.obj");
	} else if (number == 5) {
		score_[index]->reset_object("fullStomach5.obj");
	} else if (number == 6) {
		score_[index]->reset_object("fullStomach6.obj");
	} else if (number == 7) {
		score_[index]->reset_object("fullStomach7.obj");
	} else if (number == 8) {
		score_[index]->reset_object("fullStomach8.obj");
	} else if (number == 9) {
		score_[index]->reset_object("fullStomach9.obj");
	} else if (number == 0) {
		score_[index]->reset_object("fullStomach1.obj");
	}
}

#ifdef _DEBUG
#include <externals/imgui/imgui.h>
void SatisFactionLevel::Debug_gui() {
	ImGui::Begin("SatisFactionLevel");
	satisFaction_->debug_gui();
	percent_->debug_gui();
	ImGui::End();
}
#endif // _DEBUG