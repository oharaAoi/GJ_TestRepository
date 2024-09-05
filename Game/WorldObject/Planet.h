#pragma once
#include "Engine/Game/GameObject/GameObject.h"
#include "Engine/Math/Vector3.h"

/// <summary>
/// playerやEnemyが上に存在しているobject
/// </summary>
class Planet :
    public GameObject {
public:

    Planet();
    ~Planet();

    void Init();

    void Update();

private:



};

