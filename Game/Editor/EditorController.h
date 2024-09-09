#pragma once

#ifdef _DEBUG

#include <memory>

#include "Game/Editor/EnemyPopEditor/EnemyPopEditor.h"
#include "Game/Editor/MeteoritePopEditor/MeteoritePopEditor.h"

class EditorController {
public:
	EditorController() = default;
	~EditorController() = default;

	EditorController(const EditorController&) = delete;
	EditorController& operator=(const EditorController&) = delete;

public:
	void initialize(
		const Camera3D* camera,
		MeteoriteManager* meteoManager
	);
	void draw_gui();
	void draw_debug3d();

public:
	bool is_edit() const;

private:
	void main_gui();

private:
	enum class EditState {
		None,
		Enemy,
		Meteorite
	} editState;

	bool isEdit;
	bool isPlayDemoEnemy;
	std::unique_ptr<EnemyPopEditor> enemyEditor;
	bool isPlayDemoMeteorite;
	std::unique_ptr<MeteoritePopEditor> meteoriteEditor;

	bool isPlayDemo;

	const Camera3D* camera;
};

#endif // _DEBUG
