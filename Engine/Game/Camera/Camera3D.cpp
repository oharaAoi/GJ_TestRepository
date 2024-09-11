#include "Camera3D.h"

#include <cmath>

#include "Engine/WinApp.h"

#include "Engine/Game/Hierarchy/Hierarchy.h"

#ifdef _DEBUG
#include <externals/imgui/imgui.h>
#include <Game/Input/Input.h>
#endif // _DEBUG

void Camera3D::initialize() {
	set_perspective_fov_info(
		0.45f,
		static_cast<float>(WinApp::GetClientWidth()) / static_cast<float>(WinApp::GetClientHight()),
		0.1f, 1000
	);

	isVaildDebugCamera = false;
	notuseTransform = std::make_unique<Transform3D>();
	debugCameraCenter = std::make_unique<Transform3D>();
	debugCameraCenterHierarchy = std::make_unique<Hierarchy>();
	debugCameraCenterHierarchy->initialize(debugCameraCenterMatrix);
}

void Camera3D::update_matrix() {
	// リリース時は通常更新
	make_view_matrix();
	make_perspectivefov_matrix();
	vpMatrix = viewMatrix * perspectiveFovMatrix;
}

void Camera3D::set_transform(const Transform3D& transform_) noexcept {
	transform->copy(transform_);
}

void Camera3D::set_perspective_fov_info(float fovY_, float aspectRatio_, float nearClip_, float farClip_) noexcept {
	fovY = fovY_;
	aspectRatio = aspectRatio_;
	nearClip = nearClip_;
	farClip = farClip_;
}

const Matrix4x4& Camera3D::vp_matrix() const {
	return vpMatrix;
}

#ifdef _DEBUG
void Camera3D::debug_gui() {
	transform->debug_gui();
	ImGui::Separator();
	if (ImGui::Checkbox("DebugCamera", &isVaildDebugCamera)) {
		std::swap(transform, notuseTransform);
		if (isVaildDebugCamera) {
			hierarchy->set_parent(*debugCameraCenterHierarchy);
		}
		else {
			hierarchy->reset_parent();
		}
	}
	if (isVaildDebugCamera) {
		ImGui::DragFloat("Offset", &offset.z, 0.1f, -10000.0f, 0.0f);
		debugCameraCenter->debug_gui();
	}
	debug_camera();
}

void Camera3D::debug_camera() {
	if (isVaildDebugCamera) {
		// 注視距離設定
		int wheel = Input::GetWheel();
		offset.z = std::min(offset.z + wheel * 0.004f, 0.0f);
		if (Input::IsTriggerMouse(0) || Input::IsTriggerMouse(2)) {
			preMousePos = Input::GetMousePosition();
		}
		if (!ImGui::GetIO().WantCaptureMouse) {
			if (Input::IsPressMouse(0)) {
				Vector2 mousePos = Input::GetMousePosition();
				Vector2 mouseMove = mousePos- preMousePos;
				preMousePos = mousePos;

				Vector2 rotateAngle = mouseMove / 200;

				Quaternion rotation = transform->get_quaternion();
				Quaternion holizontal = Quaternion::AngleAxis(CVector3::BASIS_Y, rotateAngle.x);
				Quaternion vertical = Quaternion::AngleAxis(CVector3::BASIS_X, rotateAngle.y);

				transform->set_rotate(holizontal * rotation * vertical);
			}
			else if (Input::IsPressMouse(2)) {
				Vector2 mousePos = Input::GetMousePosition();
				Vector2 mouseMove = mousePos - preMousePos;
				preMousePos = mousePos;
				Vector3 move = mouseMove.convert(0) / -100;
				move.y *= -1;
				debugCameraCenter->plus_translate(move * transform->get_quaternion());
			}
		}

		// 位置更新
		transform->set_translate(offset * transform->get_quaternion());
		debugCameraCenterMatrix = debugCameraCenter->get_matrix();
	}
}
#endif // _DEBUG

void Camera3D::make_view_matrix() {
	viewMatrix = world_matrix().inverse();
}

void Camera3D::make_perspectivefov_matrix() {
	float cot = 1 / std::tan(fovY / 2);
	perspectiveFovMatrix = {
		{{ cot / aspectRatio, 0, 0, 0 },
		{ 0, cot, 0, 0 },
		{ 0, 0, farClip / (farClip - nearClip), 1 },
		{ 0, 0, -nearClip * farClip / (farClip - nearClip), 0 } }
	};
}

