#pragma once
#include "Engine/Game/GameObject/SpriteObject.h"

class Transform2D;

class UIObject
: public SpriteObject{
public:

	UIObject(const std::string& textureName, const Vector2& pivot);
	~UIObject();

	void Init();

	void Update(const Vector2& scale, const Vector2& translate, const float& alpha = 1);

private:



};

