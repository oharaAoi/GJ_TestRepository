#pragma once
#include <string>
#include "Engine/Math/Vector3.h"

class Enemy;

class BaseEnemyState {
public:

	// コンストラクタ
	BaseEnemyState(const std::string& name, Enemy* enemy) : name_(name), enemy_(enemy) {};
	virtual ~BaseEnemyState() {};

public:

	virtual void Update() = 0;
	virtual void Draw() = 0;

protected:
	// 状態名
	std::string name_;
	// 操作対象の敵
	Enemy* enemy_ = nullptr;
};

