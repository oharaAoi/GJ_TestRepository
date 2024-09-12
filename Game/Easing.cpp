#include "Easing.h"


namespace EaseIn {
	float Sine(float t) {return 1.0f - cos((t * PI) / 2.0f);}
	float Quad(float t) {return t * t;}
	float Cubic(float t) {return t * t * t;}
	float Quart(float t) {return t * t * t * t;}
	float Quint(float t) {return t * t * t * t * t;}
	float Expo(float t) {return t == 0.0f ? 0.0f : pow(2.0f, 10.0f * (t - 1.0f));}
	float Circ(float t) {return 1.0f - sqrt(1.0f - (t * t));}
	float Back(float t) {
		const float c1 = 1.70158f;
		const float c3 = c1 + 1.0f;
		return c3 * t * t * t - c1 * t * t;
	}

	float Elastic(float t) {
		const float c4 = (2.0f * PI) / 3.0f;
		return t == 0.0f ? 0.0f : t == 1.0f ? 1.0f : -pow(2.0f, 10.0f * t - 10.0f) * sin((t * 10.0f - 10.75f) * c4);
	}

	float Bounce(float t) {
		return 1.0f - EaseOut::Bounce(1.0f - t);
	}
}

namespace EaseOut {
	float Sine(float t) {return sin((t * PI) / 2.0f);}
	float Quad(float t) {return t * (2.0f - t);}
	float Cubic(float t) {return 1.0f - pow(1.0f - t, 3.0f);}
	float Quart(float t) {return 1.0f - pow(1.0f - t, 4.0f);}
	float Quint(float t) {return 1.0f - pow(1.0f - t, 5.0f);}
	float Expo(float t) {return t == 1.0f ? 1.0f : 1.0f - pow(2.0f, -10.0f * t);}
	float Circ(float t) {return sqrt(1.0f - pow(t - 1.0f, 2.0f));}
	float Back(float t) {
		const float c1 = 1.70158f;
		const float c3 = c1 + 1.0f;
		return 1.0f + c3 * pow(t - 1.0f, 3.0f) + c1 * pow(t - 1.0f, 2.0f);
	}

	float Elastic(float t) {
		const float c4 = (2.0f * PI) / 3.0f;
		return t == 0.0f ? 0.0f : t == 1.0f ? 1.0f : pow(2.0f, -10.0f * t) * sin((t * 10.0f - 0.75f) * c4) + 1.0f;
	}

	float Bounce(float t) {
		if (t < 1.0f / 2.75f) {
			return 7.5625f * t * t;
		} else if (t < 2.0f / 2.75f) {
			t -= 1.5f / 2.75f;
			return 7.5625f * t * t + 0.75f;
		} else if (t < 2.5f / 2.75f) {
			t -= 2.25f / 2.75f;
			return 7.5625f * t * t + 0.9375f;
		} else {
			t -= 2.625f / 2.75f;
			return 7.5625f * t * t + 0.984375f;
		}
	}
}

namespace EaseInOut {
	float Sine(float t) {return -(cos(PI * t) - 1.0f) / 2.0f;}
	float Quad(float t) {return t < 0.5f ? 2.0f * t * t : 1.0f - pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;}
	float Cubic(float t) {return t < 0.5f ? 4.0f * t * t * t : 1.0f - pow(-2.0f * t + 2.0f, 3.0f) / 2.0f;}
	float Quart(float t) {return t < 0.5f ? 8.0f * t * t * t * t : 1.0f - pow(-2.0f * t + 2.0f, 4.0f) / 2.0f;}
	float Quint(float t) {return t < 0.5f ? 16.0f * t * t * t * t * t : 1.0f - pow(-2.0f * t + 2.0f, 5.0f) / 2.0f;}
	float Expo(float t) {
		return t == 0.0f ? 0.0f : t == 1.0f ? 1.0f : t < 0.5f ? pow(2.0f, 20.0f * t - 10.0f) / 2.0f : (2.0f - pow(2.0f, -20.0f * t + 10.0f)) / 2.0f;
	}

	float Circ(float t) {
		return t < 0.5f ? (1.0f - sqrt(1.0f - pow(2.0f * t, 2.0f))) / 2.0f : (sqrt(1.0f - pow(-2.0f * t + 2.0f, 2.0f)) + 1.0f) / 2.0f;
	}

	float Back(float t) {
		const float c1 = 1.70158f;
		const float c3 = c1 * 1.525f;
		return t < 0.5f
			? (powf(2.0f * t, 2.0f) * ((c3 + 1) * 2.0f * t - c3)) / 2.0f
			: (powf(2.0f * t - 2.0f, 2.0f) * ((c3 + 1.0f) * (t * 2.0f - 2.0f) + c3) + 2.0f) / 2.0f;
	}

	float Elastic(float t) {
		const float c5 = (2.0f * PI) / 4.5f;
		return t == 0.0f ? 0.0f : t == 1.0f ? 1.0f : t < 0.5f ? -(pow(2.0f, 20.0f * t - 10.0f) * sin((20.0f * t - 11.125f) * c5)) / 2.0f : (pow(2.0f, -20.0f * t + 10.0f) * sin((20.0f * t - 11.125f) * c5)) / 2.0f + 1.0f;
	}

	float Bounce(float t) {
		return t < 0.5f ? (1.0f - EaseOut::Bounce(1.0f - 2.0f * t)) / 2.0f : (1.0f + EaseOut::Bounce(2.0f * t - 1.0f)) / 2.0f;
	}
}