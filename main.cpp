#include "Engine/WinApp.h"

#include <cstdint>

#include "Engine/Render/RenderPathManager/RenderPathManager.h"
#include "TestCode/SceneDemo.h"
#include "Game/Scene/GameScene.h"
#include "Engine/Game/Managers/SceneManager/SceneManager.h"
#include "Engine/Utility/Utility.h"
#include "TestCode/SceneDemo.h"

#include "Engine/Game/Managers/PolygonMeshManager/PolygonMeshManager.h"
#include "Engine/Utility//BackgroundLoader/BackgroundLoader.h"

// クライアント領域サイズ
const std::int32_t kClientWidth = 1280;
const std::int32_t kClientHight = 720;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp::Initialize("DirectXGame", kClientWidth, kClientHight);

	auto&& scene = CreateUnique<GameScene>();
	SceneManager::Initialize(std::move(scene));



	while (!WinApp::IsEndApp()) {
		WinApp::BeginFrame();

		SceneManager::Begin();

		if (SceneManager::IsEndProgram()) {
			break;
		}

		SceneManager::Update();

		SceneManager::Draw();

		SceneManager::DebugGui();

		WinApp::EndFrame();
	}

	SceneManager::Finalize();

	WinApp::Finalize();
}