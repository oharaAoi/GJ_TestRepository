#pragma once
#include "Engine/Game/GameObject/SpriteObject.h"

class PlayerUI {

public:

    PlayerUI();
    ~PlayerUI();

    void Init();

    void Update();

    void BeginRendering();

    void Draw() const ;

private:

    SpriteObject playerUI{ "HP_bar.png", {0.5f, 0.5f} };

};

