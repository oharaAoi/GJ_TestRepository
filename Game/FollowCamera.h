#pragma once

#include "Engine/Game/Camera/Camera3D.h"

class Player;

class FollowCamera : public Camera3D {
public:
	void initialize() override;
	void update() override;

	void SetOffset(const Vector3& offset_);
	void SetPlayer(const Player* player);

private:
	Vector3 offset;

	const Player* pPlayer;
};
