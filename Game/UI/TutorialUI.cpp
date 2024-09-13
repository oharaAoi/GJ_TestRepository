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
	basePos_ = { 320.0f, 560.0f };
	taskPos_ = basePos_;

	skip_ = std::make_unique<UIObject>("skipKey.png", ancherPoint);

	content_UI = std::make_unique<UIObject>("tutorial.png", ancherPoint);
	woodBord_UI = std::make_unique<UIObject>("woodBoard.png", ancherPoint);

	makimonoOrigine_UI = std::make_unique<UIObject>("scrollStart.png", ancherPoint);
	makimonoEnd_UI = std::make_unique<UIObject>("scrollEnd.png", ancherPoint);

	makimono_UI = std::make_unique<UIObject>("scroll.png", ancherPoint);
	makimono_UI->Update({ 0.5f, 0.5f }, basePos_);

	task_ = std::make_unique<UIObject>("scroll.png", ancherPoint);

	adjustmentItem_ = AdjustmentItem::GetInstance();
	const char* groupName = "TutorialUI";
	adjustmentItem_->AddItem(groupName, "move", Vector2{300.0f,580.0f});
	adjustmentItem_->AddItem(groupName, "appearance", taskPos_);
	adjustmentItem_->AddItem(groupName, "attract", taskPos_);
	adjustmentItem_->AddItem(groupName, "attractEnemy", taskPos_);
	adjustmentItem_->AddItem(groupName, "collision", taskPos_);
	adjustmentItem_->AddItem(groupName, "collisionEnemy", taskPos_);
	adjustmentItem_->AddItem(groupName, "collisionEnemy", taskPos_);
	adjustmentItem_->AddItem(groupName, "fall", taskPos_);
	adjustmentItem_->AddItem(groupName, "kick", taskPos_);
	adjustmentItem_->AddItem(groupName, "putAway", taskPos_);
	adjustmentItem_->AddItem(groupName, "putOut", taskPos_);
	adjustmentItem_->AddItem(groupName, "rotate", taskPos_);
	adjustmentItem_->AddItem(groupName, "complete", taskPos_);

	uiMoveStartPos = taskPos_;
	uiMoveEndPos = { -320.0f, taskPos_.y };
}

void TutorialUI::Update(const int& contentNum) {
	skip_->Update({ 0.4f,0.4f }, { 1070, 630 });
	content_UI->Update({ 0.4f,0.4f }, { 250, 87 });
	woodBord_UI->Update({ 0.4f, 0.4f }, { 250, 87 });
	//makimono_UI->Update({ 0.5f, 0.5f }, basePos_);

	adjustmentItem_->Update();

	//switch (contentNum) {
	//case TutorialContent::FirstMove_Content:
	//	tips_->Update(scale_, tipsPos_);
	//	task_->Update(scale_, taskPos_);
	//break;

	//case TutorialContent::RodPutOn_Content:
	//	tips_->Update(scale_, tipsPos_);
	//	task_->Update(scale_, taskPos_);
	//	break;
	//case TutorialContent::MeteoCollision_Content:
	//	tips_->Update(scale_, tipsPos_);
	//	task_->Update(scale_, taskPos_);
	//	break;
	//case TutorialContent::CantMoveCanRotate_Content:
	//	tips_->Update(scale_, tipsPos_);
	//	task_->Update(scale_, taskPos_);
	//	break;

	//case TutorialContent::FirstEnemy_Content:
	//	tips_->Update(scale_, tipsPos_);
	//	task_->Update(scale_, taskPos_);
	//	break;
	//case TutorialContent::EnemyCollisionToMeteo_Content:
	//	tips_->Update(scale_, tipsPos_);
	//	task_->Update(scale_, taskPos_);
	//	break;
	//case TutorialContent::MeteoAttract_Content:
	//	break;
	//}
	if (isEndTask) {
		endTaskAnimationTimer += GameTimer::DeltaTime();
		float parametric = (std::min)(1.0f, endTaskAnimationTimer / EndTaskAnimationTime);
		taskPos_ = Vector2::Lerp(uiMoveStartPos, uiMoveEndPos, EaseIn::Back(parametric));
		makimono_UI->Update({ 0.5f, 0.5f }, Vector2::Lerp(uiMoveStartPos, uiMoveEndPos, EaseIn::Back(parametric)));
	}
	task_->Update(scale_, taskPos_);
}

void TutorialUI::BeginRendering() {
	skip_->begin_rendering();
	content_UI->begin_rendering();
	woodBord_UI->begin_rendering();
	/*makimonoOrigine_UI->begin_rendering();
	makimonoEnd_UI->begin_rendering();*/
	makimono_UI->begin_rendering();

	task_->begin_rendering();
}

void TutorialUI::Draw() const {
	skip_->draw();
	woodBord_UI->draw();
	content_UI->draw();
	/*makimonoOrigine_UI->draw();
	makimonoEnd_UI->draw();*/
	makimono_UI->draw();

	task_->draw();
}

void TutorialUI::ChangeContentUI(const int& contentNum) {
	const char* groupName = "TutorialUI";
	switch (contentNum) {
	case TutorialContent::FirstMove_Content:
		task_.reset(new UIObject("move.png", ancherPoint));
		scale_ = { 0.4f, 0.4f };
		taskPos_ = {300.0f,580 };
		break;

	case TutorialContent::RodPutOn_Content:
		task_.reset(new UIObject("putOut.png", ancherPoint));
		scale_ = { 0.4f, 0.4f };
		taskPos_ = { 320.0f,575.0f };
		break;
	case TutorialContent::MeteoCollision_Content:
		task_.reset(new UIObject("collision.png", ancherPoint));
		scale_ = { 0.4f, 0.4f };
		taskPos_ = { 320.0f,575.0f };
		break;
	case TutorialContent::CantMoveCanRotate_Content:
		task_.reset(new UIObject("putAway.png", ancherPoint));
		scale_ = { 0.4f, 0.4f };
		taskPos_ = { 320.0f,575.0f };
		break;

	case TutorialContent::FirstEnemy_Content:
		task_.reset(new UIObject("kick.png", ancherPoint));
		scale_ = { 0.4f, 0.4f };
		taskPos_ = { 320.0f,575.0f };

		break;
	case TutorialContent::EnemyCollisionToMeteo_Content:
		task_.reset(new UIObject("collisionEnemy.png", ancherPoint));
		scale_ = { 0.4f, 0.4f };
		taskPos_ = { 320.0f,575.0f };
		break;
	case TutorialContent::MeteoAttract_Content:
		task_.reset(new UIObject("complete.png", ancherPoint));
		scale_ = { 0.4f, 0.4f };
		taskPos_ = { 320.0f, 560.0f };
		content_UI.reset(new UIObject("practiceRange.png", ancherPoint));
		break;
	}
	task_->Update(scale_, taskPos_);
}

void TutorialUI::SetEndTask() {
	isEndTask = true;
}

#ifdef _DEBUG
void TutorialUI::EditGui() {
	ImGui::DragFloat2("TaskPos", &taskPos_.x);
	ImGui::DragFloat2("BasePos", &basePos_.x);
	skip_->debug_gui();
	woodBord_UI->debug_gui();
	makimono_UI->debug_gui();
}
#endif // DEBUG