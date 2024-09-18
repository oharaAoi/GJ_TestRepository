#pragma once

#include <chrono>
#include <list>

class GameTimer {
private:
	GameTimer() = default;

public:
	~GameTimer() = default;

	GameTimer(const GameTimer&) = delete;
	GameTimer& operator=(const GameTimer&) = delete;

public:
	static GameTimer& GetInstance();

	static void Initialize();
	static void Update();

public:
	static float DeltaTime();
	static float AverageFPS();
	static const std::chrono::high_resolution_clock::time_point& BeginTime();

#ifdef _DEBUG
public:
	static void IsFixDeltaTime(bool boolean);
#endif // _DEBUG

private:
	float deltaTime;
	std::chrono::high_resolution_clock::time_point startFrameTimePoint;

	std::list<float> deltaTimeList;
	float timeSummation;
	float fps;

	bool isFixDeltaTime;
};

