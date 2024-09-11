#pragma once
#include <cmath>
#include "Engine/Math/Definition.h"

namespace EaseIn {
    float Sine(float t);
    float Quad(float t);
    float Cubic(float t);
    float Quart(float t);
    float Quint(float t);
    float Expo(float t);
    float Circ(float t);
    float Back(float t);
    float Elastic(float t);
    float Bounce(float t);
}

namespace EaseOut {
    float Sine(float t);
    float Quad(float t);
    float Cubic(float t);
    float Quart(float t);
    float Quint(float t);
    float Expo(float t);
    float Circ(float t);
    float Back(float t);
    float Elastic(float t);
    float Bounce(float t);
}

namespace EaseInOut {
    float Sine(float t);
    float Quad(float t);
    float Cubic(float t);
    float Quart(float t);
    float Quint(float t);
    float Expo(float t);
    float Circ(float t);
    float Back(float t);
    float Elastic(float t);
    float Bounce(float t);
}