#include "Billboard.h"

Billboard::Billboard(const Vector3& pos) {
	Init(pos);
}

Billboard::~Billboard() {
}

void Billboard::Init(const Vector3& pos) {
	reset_object("attract.obj");
	transform->set_translate(pos);
	transform->set_scale(RandomVector3(3.5f, 5.0f));
	velocity_ = RandomVector3(-1, 1);
	accelaration_ = velocity_.normalize_safe();
	lifeTime_ = 60.0f;
	isDead_ = false;
}

void Billboard::Update(const Vector3& camera3DTranslate) {
	// 移動をさせる
	Vector3 translate = transform->get_translate();
	accelaration_ += accelaration_ * GameTimer::DeltaTime();
	velocity_ += accelaration_ * GameTimer::DeltaTime();
	translate += velocity_;
	transform->set_translate(translate);

	// 板ポリをカメラに向ける処理を行う
	// カメラと板ポリの距離を測る
	Vector3 distance = (camera3DTranslate - world_position()).normalize_safe();
	// カメラとのベクトルが板ポリを向ける方向になる
	Quaternion boardRotate = transform->get_quaternion();
	// 板ポリの前方方向を取得する
	Quaternion lookCameraRotate = Quaternion::FromToRotation(Vector3{ 0.0f, 0.0f, 1.0f }, distance);
	// 向きを整える
	transform->set_rotate(lookCameraRotate);

	if (--lifeTime_ < 0) {
		isDead_ = true;
	}
}

void Billboard::Begine_Rendering(const Camera3D& camera3D) {
	begin_rendering(camera3D);
}

void Billboard::Draw() {
	draw();
}
