#pragma once

#include "Engine/Game/GameObject/SpriteObject.h"

/// <summary>
/// GameOver時のUI
/// </summary>
class GameOverUI {
public:

	GameOverUI();
	~GameOverUI();

	void Init();

	void Update();

	void Begin_Rendering();

	void Draw() const;


#ifdef _DEBUG
	void Debug_gui();
#endif // _DEBUG

private:

	SpriteObject goTitle{ "UI_goGame.png",  {0.5f, 0.5f} };
	SpriteObject goGame{ "UI_goGame.png",  {0.5f, 0.5f} };

};

