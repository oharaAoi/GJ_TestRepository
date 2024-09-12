#pragma once

#include "Engine/Game/GameObject/SpriteObject.h"
#include "Game/UI/UIObject.h"

/// <summary>
/// GameOver時のUI
/// </summary>
class GameOverUI {
public:

	struct Sprite : public SpriteObject {
		Sprite(const std::string& textureName, const Vector2& pivot) :
			SpriteObject(textureName, pivot){
		};

		
	};

public:

	GameOverUI();
	~GameOverUI();

	void Init();

	void Update(const bool& nextGame);

	void Begin_Rendering();

	void Draw() const;


#ifdef _DEBUG
	void Debug_gui();
#endif // _DEBUG

private:

	std::unique_ptr<UIObject> goTitle_ = nullptr;
	std::unique_ptr<UIObject> goGame_ = nullptr;
	std::unique_ptr<UIObject> arrow_ = nullptr;
};

