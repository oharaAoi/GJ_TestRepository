#pragma once
#include <random>
#include "Engine/Math/Vector3.h"

inline std::random_device seedGenerator;
inline std::mt19937 randomEngine(seedGenerator());

int RandomInt(const int& min, const int& max);

float RandomFloat(const float& min, const float& max);

Vector3 RandomVector3(const float& min, const float& max);