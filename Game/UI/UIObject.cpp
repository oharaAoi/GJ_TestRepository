#include "UIObject.h"
#include "Engine/Game/Transform2D/Transform2D.h"

UIObject::UIObject(const std::string& textureName, const Vector2& pivot)
	: SpriteObject(textureName, pivot) {
	
}

UIObject::~UIObject() {
}

void UIObject::Init() {
}

void UIObject::Update(const Vector2& scale, const Vector2& translate, const float& alpha) {
	transform->set_scale(scale);
	transform->set_translate(translate);
	color.alpha = alpha;
}