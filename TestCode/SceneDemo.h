#pragma once
#include "Engine/Game/Scene/BaseScene.h"
#include "Engine/Game/Camera/Camera3D.h"
#include "Engine/Render/RenderPathManager/RenderPathManager.h"

class SceneDemo : public BaseScene {
public:
	SceneDemo() = default;
	~SceneDemo() = default;

public:
	void load();
	void initialize();
	void finalize();

	void begin() override;
	void update() override;
	void begin_rendering() override;
	void late_update() override;

	void draw() const override;

#ifdef _DEBUG
	void debug_update() override;
#endif // _DEBUG

private:



};
