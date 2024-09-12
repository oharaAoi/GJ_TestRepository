#pragma once
#ifdef _DEBUG

class Camera3D;

#include <string>

#include "Game/WorldObject/MeteoriteManager.h"

class MeteoritePopEditor {
public:
	MeteoritePopEditor() = default;
	~MeteoritePopEditor() = default;

	MeteoritePopEditor(const MeteoritePopEditor&) = delete;
	MeteoritePopEditor& operator=(const MeteoritePopEditor&) = delete;

public:
	void initialize(MeteoriteManager* manager_);

	void draw_editor();
	void debug_draw3d(const Camera3D* camera);

	void run_demo_play();
	void stop_demo_play();

private:
	void edit_window();
	void group_config();
	std::string save(const std::string& fileName);
	void compress();
	void decompress();
	void get_group_map_keys();

private:
	MeteoriteManager* manager;

	std::vector<std::string> fileList;
	char exportFileName[64];
	std::string inportFileName;
	std::string resultString;

	MeteoriteManager::Group editGroup;
	float repopTime;
	float popWidth;
	std::vector<Vector3> popList;

	std::vector<GameObject> debugDrawObject;
};
#endif // _DEBUG
