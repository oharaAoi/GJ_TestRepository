#pragma once
#include "Engine/Game/GameObject/SpriteObject.h"
#include "Game/UI/UIObject.h"

/// <summary>
/// シーンフェードのパネル
/// </summary>
class FadePanel {
public:

	FadePanel();
	~FadePanel();

	void Init();

	void Update();

	void Begin_Rendering();

	void Draw() const;

#ifdef _DEBUG
	void Debug_gui();
#endif // _DEBUG


private:

	std::unique_ptr<UIObject> fadePanel_ = nullptr;

};

