#include "TutorialUI.h"
#include "Game/Scene/TutorialScene.h"

TutorialUI::TutorialUI() {
	Init();
}

TutorialUI::~TutorialUI() {
}

void TutorialUI::Finalize() {
}

void TutorialUI::Init() {
	content_UI = std::make_unique<UIObject>("tutorial.png", ancherPoint);
	woodBord_UI = std::make_unique<UIObject>("woodBoard.png", ancherPoint);

	makimonoOrigine_UI = std::make_unique<UIObject>("scrollStart.png", ancherPoint);
	makimonoEnd_UI = std::make_unique<UIObject>("scrollEnd.png", ancherPoint);

	makimono_UI = std::make_unique<UIObject>("scroll.png", ancherPoint);

	tips_ = std::make_unique<UIObject>("null.png", ancherPoint);
	task_ = std::make_unique<UIObject>("scroll.png", ancherPoint);

	tipsPos_ = { 0,0 };
	taskPos_ = { 0,0 };

	basePos_ = { 320.0f, 530.0f };

	adjustmentItem_ = AdjustmentItem::GetInstance();
	const char* groupName = "TutorialUI";
	adjustmentItem_->AddItem(groupName, "move", tipsPos_);
	adjustmentItem_->AddItem(groupName, "appearance", tipsPos_);
	adjustmentItem_->AddItem(groupName, "attract", tipsPos_);
	adjustmentItem_->AddItem(groupName, "attractEnemy", tipsPos_);
	adjustmentItem_->AddItem(groupName, "collision", tipsPos_);
	adjustmentItem_->AddItem(groupName, "collisionEnemy", tipsPos_);
	adjustmentItem_->AddItem(groupName, "collisionEnemy", tipsPos_);
	adjustmentItem_->AddItem(groupName, "fall", tipsPos_);
	adjustmentItem_->AddItem(groupName, "fall", tipsPos_);
	adjustmentItem_->AddItem(groupName, "fall", tipsPos_);
	adjustmentItem_->AddItem(groupName, "kick", tipsPos_);
	adjustmentItem_->AddItem(groupName, "putAway", tipsPos_);
	adjustmentItem_->AddItem(groupName, "putOut", tipsPos_);
	adjustmentItem_->AddItem(groupName, "rotate", tipsPos_);
	adjustmentItem_->AddItem(groupName, "complete", tipsPos_);
	

}

void TutorialUI::Update(const int& contentNum) {
	content_UI->Update({ 0.4f,0.4f }, { 640, 87 });
	woodBord_UI->Update({ 0.4f, 0.4f }, { 640, 87 });
	makimono_UI->Update({ 0.5f, 0.5f }, basePos_);

	adjustmentItem_->Update();

	switch (contentNum) {
	case TutorialContent::FirstMove_Content:
		tips_->Update(scale_, tipsPos_);
		task_->Update(scale_, taskPos_);
	break;

	case TutorialContent::RodPutOn_Content:
		tips_->Update(scale_, tipsPos_);
		task_->Update(scale_, taskPos_);
		break;
	case TutorialContent::MeteoCollision_Content:
		tips_->Update(scale_, tipsPos_);
		task_->Update(scale_, taskPos_);
		break;
	case TutorialContent::CantMoveCanRotate_Content:
		tips_->Update(scale_, tipsPos_);
		task_->Update(scale_, taskPos_);
		break;

	case TutorialContent::FirstEnemy_Content:
		tips_->Update(scale_, tipsPos_);
		task_->Update(scale_, taskPos_);
		break;
	case TutorialContent::EnemyCollisionToMeteo_Content:
		tips_->Update(scale_, tipsPos_);
		task_->Update(scale_, taskPos_);
		break;
	case TutorialContent::MeteoAttract_Content:
		tips_->Update(scale_, tipsPos_);
		task_->Update(scale_, taskPos_);
		break;
	}
}

void TutorialUI::BeginRendering() {
	content_UI->begin_rendering();
	woodBord_UI->begin_rendering();
	/*makimonoOrigine_UI->begin_rendering();
	makimonoEnd_UI->begin_rendering();*/
	makimono_UI->begin_rendering();

	task_->begin_rendering();
	tips_->begin_rendering();
}

void TutorialUI::Draw() const {
	woodBord_UI->draw();
	content_UI->draw();
	/*makimonoOrigine_UI->draw();
	makimonoEnd_UI->draw();*/
	makimono_UI->draw();

	tips_->draw();
	task_->draw();
}

void TutorialUI::ChangeContentUI(const int& contentNum) {
	const char* groupName = "TutorialUI";
	switch (contentNum) {
	case TutorialContent::FirstMove_Content:
		tips_.reset(new UIObject("null.png", ancherPoint));
		task_.reset(new UIObject("move.png", ancherPoint));
		scale_ = { 0.4f, 0.4f };
		tipsPos_ = { 0.0f,0.0f };
		taskPos_ = adjustmentItem_->GetValue<Vector2>(groupName, "move");
		break;

	case TutorialContent::RodPutOn_Content:
		tips_.reset(new UIObject("attract.png", ancherPoint));
		task_.reset(new UIObject("putOut.png", ancherPoint));
		scale_ = { 0.4f, 0.4f };
		tipsPos_ = adjustmentItem_->GetValue<Vector2>(groupName, "attract");
		taskPos_ = adjustmentItem_->GetValue<Vector2>(groupName, "putOut");
		break;
	case TutorialContent::MeteoCollision_Content:
		tips_.reset(new UIObject("fall.png", ancherPoint));
		task_.reset(new UIObject("collision.png", ancherPoint));
		scale_ = { 0.4f, 0.4f };
		tipsPos_ = adjustmentItem_->GetValue<Vector2>(groupName, "fall");
		taskPos_ = adjustmentItem_->GetValue<Vector2>(groupName, "collision");
		break;
	case TutorialContent::CantMoveCanRotate_Content:
		tips_.reset(new UIObject("rotate.png", ancherPoint));
		task_.reset(new UIObject("putAway.png", ancherPoint));
		scale_ = { 0.4f, 0.4f };
		tipsPos_ = adjustmentItem_->GetValue<Vector2>(groupName, "rotate");
		taskPos_ = adjustmentItem_->GetValue<Vector2>(groupName, "putAway");
		break;

	case TutorialContent::FirstEnemy_Content:
		tips_.reset(new UIObject("appearance.png", ancherPoint));
		task_.reset(new UIObject("kick.png", ancherPoint));
		scale_ = { 0.4f, 0.4f };
		tipsPos_ = adjustmentItem_->GetValue<Vector2>(groupName, "appearance");
		taskPos_ = adjustmentItem_->GetValue<Vector2>(groupName, "kick");

		break;
	case TutorialContent::EnemyCollisionToMeteo_Content:
		tips_.reset(new UIObject("attractEnemy.png", ancherPoint));
		task_.reset(new UIObject("collisionEnemy.png", ancherPoint));
		scale_ = { 0.3f, 0.3f };
		tipsPos_ = adjustmentItem_->GetValue<Vector2>(groupName, "attractEnemy");
		taskPos_ = adjustmentItem_->GetValue<Vector2>(groupName, "collisionEnemy");
		break;
	case TutorialContent::MeteoAttract_Content:
		tips_.reset(new UIObject("complete.png", ancherPoint));
		task_.reset(new UIObject("collisionEnemy.png", ancherPoint));
		scale_ = { 0.4f, 0.4f };
		tipsPos_ = adjustmentItem_->GetValue<Vector2>(groupName, "complete");
		taskPos_ = {-300.0f, -400.0f};
		content_UI.reset(new UIObject("practiceRange.png", ancherPoint));
		break;
	}
}

#ifdef _DEBUG
void TutorialUI::EditGui() {
	ImGui::DragFloat2("TipsPos", &tipsPos_.x);
	ImGui::DragFloat2("TaskPos", &taskPos_.x);
	ImGui::DragFloat2("BasePos", &basePos_.x);
	//woodBord_UI->debug_gui();
	makimono_UI->debug_gui();
}
#endif // DEBUG