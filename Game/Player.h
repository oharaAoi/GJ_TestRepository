#pragma once
#include "Engine/GameObject/GameObject.h"
#include "Game/Input/Input.h"
#include "Engine/Math/Vector3.h"

class Transform3D;

class Player :
    public GameObject {

public:

    Player();
    ~Player();

    void Init();

    void Update();

private:



};

