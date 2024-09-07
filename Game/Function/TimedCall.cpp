#include "TimedCall.h"
#include <externals/imgui/imgui.h>

TimedCall::TimedCall(std::function<void(void)> callback, const uint32_t& time) :
callback_(callback),
time_(time)
{}

TimedCall::~TimedCall() {
}

void TimedCall::Init() {
}

void TimedCall::Update() {
	// 完了なら
	if (isFinish_) {
		return;
	}

	time_--;

#ifdef _DEBUG
	ImGui::Begin("TimedCall");
	ImGui::Text("time %d", time_);
	ImGui::End();
#endif

	if (time_ <= 0) {
		isFinish_ = true;
		// コールバック関数の呼び出し
		callback_();
	}
}
