#pragma once
#include <vector>
#include <unordered_map>
#include "Engine/Game/GameObject/SpriteObject.h"
#include "Game/UI/UIObject.h"

#include "Game/GameCharacter/Manager/AdjustmentItem.h"

class Transform2D;
class TutorialScene;

class TutorialUI {
public:

	struct TutorialContentTexture {
		std::string tips;
		std::string task;
	};

public:

	TutorialUI();
	~TutorialUI();

	void Finalize();

	void Init();

	void Update(const int& contentNum);

	void BeginRendering();

	void Draw() const;

#ifdef _DEBUG
	void EditGui();
#endif // DEBUG

	void ChangeContentUI(const int& contentNum);

private:

	Vector2 ancherPoint{ 0.5f, 0.5f };

	AdjustmentItem* adjustmentItem_;

	Vector2 tipsPos_;
	Vector2 taskPos_;
	Vector2 basePos_;
	Vector2 scale_;

	std::unique_ptr<UIObject> skip_;

	std::unique_ptr<UIObject> tips_;
	std::unique_ptr<UIObject> task_;

	std::unique_ptr<UIObject> content_UI = nullptr; // チュートリアルか練習か
	std::unique_ptr<UIObject> woodBord_UI = nullptr;

	// 巻物
	std::unique_ptr<UIObject> makimonoOrigine_UI = nullptr;
	std::unique_ptr<UIObject> makimonoEnd_UI = nullptr;
	std::unique_ptr<UIObject> makimono_UI = nullptr;

};

