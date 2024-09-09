#pragma once

class EnemyPopEditor {
public:
	EnemyPopEditor() = default;
	~EnemyPopEditor() = default;

	EnemyPopEditor(const EnemyPopEditor&) = delete;
	EnemyPopEditor& operator=(const EnemyPopEditor&) = delete;

public:
	void draw_editor();

	void run_demo_play();
	void stop_demo_play();

private:

private:
};

