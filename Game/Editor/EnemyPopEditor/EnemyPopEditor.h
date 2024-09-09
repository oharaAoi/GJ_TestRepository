#pragma once
#ifdef _DEBUG

class Camera3D;

#include <string>

#include "Game/GameCharacter/Manager/EnemyManager.h"

class EnemyPopEditor {
public:
	EnemyPopEditor() = default;
	~EnemyPopEditor() = default;

	EnemyPopEditor(const EnemyPopEditor&) = delete;
	EnemyPopEditor& operator=(const EnemyPopEditor&) = delete;

public:
	void initialize(EnemyManager* manager_);

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
	EnemyManager* manager;

	std::vector<std::string> fileList;
	char exportFileName[64];
	std::string inportFileName;
	std::string resultString;

	EnemyManager::Group editGroup;

	struct PopData {
		EnemyManager::SettingData data;
		GameObject object;
	};
	std::vector<PopData> editPopData;
};
#endif // _DEBUG
