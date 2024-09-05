#include "FollowCamera.h"

#include "Game/GameCharacter/Player.h"

void FollowCamera::initialize() {
	Camera3D::initialize();
	transform->set_rotate(
		Quaternion::EulerDegree(90, 0, 0)
	);
}

void FollowCamera::update() {
	const Quaternion& rotation = transform->get_quaternion();
	
}

void FollowCamera::SetOffset(const Vector3& offset_) {
	offset = offset_;
}

void FollowCamera::SetPlayer(const Player* player) {
	pPlayer = player;
}
