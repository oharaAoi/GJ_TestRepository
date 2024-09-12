#pragma once
#include "Engine/Game/GameObject/SpriteObject.h"
#include "Game/UI/UIObject.h"

class Transform2D;

class TutorialUI {
public:

	TutorialUI();
	~TutorialUI();

    void Finalize();

    void Init();

    void Update();

    void BeginRendering();

    void Draw() const;

#ifdef _DEBUG
    void EditGui();
#endif // DEBUG

private:

    Vector2 ancherPoint{ 0.5f, 0.5f };

    std::unique_ptr<UIObject> content_UI = nullptr; // チュートリアルか練習か
    std::unique_ptr<UIObject> woodBord_UI = nullptr;

    // 巻物
    std::unique_ptr<UIObject> makimonoOrigine_UI = nullptr;
    std::unique_ptr<UIObject> makimonoEnd_UI = nullptr;
    std::unique_ptr<UIObject> makimono_UI = nullptr;

};

