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

    std::unique_ptr<UIObject> content_UI = nullptr;

};

