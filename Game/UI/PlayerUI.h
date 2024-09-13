#pragma once
#include "Engine/WinApp.h"
#include "Engine/Game/GameObject/SpriteObject.h"
#include "Game/UI/UIObject.h"

class Transform2D;

class PlayerUI {

public:

    PlayerUI();
    ~PlayerUI();

    void Finalize();

    void Init();

    void Update(const Vector3& playerPos, const Matrix4x4& vpMatrix, const bool& isAttack);

    void BeginRendering();

    void Draw() const ;

#ifdef _DEBUG
    void EditGui();
#endif // DEBUG

    Matrix4x4 MakeViewportMatrix(const float& left, const float& top, const float& width, const float& height,
                            const float& minDepth, const float& maxDepth);

    Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

private:

    std::unique_ptr<UIObject> playerControl_UI = nullptr;
    std::unique_ptr<UIObject> playerAttack_UI = nullptr;
 
    bool isPlayerAttack_;

	Vector2 offsetPos_;
};

