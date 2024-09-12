#pragma once
#include <vector>
#include <unordered_map>
#include "Engine/Game/GameObject/GameObject.h"
#include "Game/UI/UIObject.h"

#include "Game/GameCharacter/Manager/AdjustmentItem.h"

class SatisFactionLevel {
public:

	SatisFactionLevel();
	~SatisFactionLevel();

	void Init();

	void Update(const int& satisFaction, const int& satisFactionLevel);

	void BeginRendering(Camera3D* camera3d);

	void Draw();

#ifdef _DEBUG
	void Debug_gui();
#endif // _DEBUG

	void ScoreChange();

	void DigitCreate();

	void NumberChange(const int& number, const int& index);

private:

	std::unique_ptr<GameObject> satisFaction_ = nullptr;
	std::unique_ptr<GameObject> percent_ = nullptr;

	std::vector<std::unique_ptr<GameObject>> score_;

	uint32_t digit_ = 1;

	float raito_;
};

