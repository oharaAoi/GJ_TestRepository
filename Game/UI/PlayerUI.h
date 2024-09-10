#pragma once
#include "Engine/WinApp.h"
#include "Engine/Game/GameObject/SpriteObject.h"

class PlayerUI {

public:

    PlayerUI();
    ~PlayerUI();

    void Finalize();

    void Init();

    void Update(const Vector3& playerPos, const Matrix4x4& vpMatrix);

    void BeginRendering();

    void Draw() const ;

#ifdef _DEBUG
    void EditGui();
#endif // DEBUG

    Matrix4x4 MakeViewportMatrix(const float& left, const float& top, const float& width, const float& height,
                            const float& minDepth, const float& maxDepth);

private:

    SpriteObject playerControl_UI{ "UI_PlayerControl.png", {0,0} };

};

