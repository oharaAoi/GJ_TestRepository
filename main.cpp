#include "Engine/WinApp.h"
#include "Engine/Math/Camera3D.h"

// クライアント領域サイズ
const int32_t kClientWidht = 1280;
const int32_t kClientHight = 720;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp::Initialize("Test", kClientWidht, kClientHight);
	Camera3D::Initialize();

	while (!WinApp::IsEndApp()) {
		WinApp::BeginFrame();
		WinApp::EndFrame();
	}

	WinApp::Finalize();
}