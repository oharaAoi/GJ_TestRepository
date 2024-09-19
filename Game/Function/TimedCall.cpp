//#include "TimedCall.h"
//#include <externals/imgui/imgui.h>
//
//Test::TimedCall::TimedCall(std::function<void(void)> callback, const uint32_t& time) :
//callback_(callback),
//time_(time)
//{}
//
//Test::TimedCall::~TimedCall() {
//}
//
//void Test::TimedCall::Init() {
//}
//
//void Test::TimedCall::Update() {
//	// 完了なら
//	if (isFinish_) {
//		return;
//	}
//
//	time_--;
//
//	if (time_ <= 0) {
//		isFinish_ = true;
//		// コールバック関数の呼び出し
//		callback_();
//	}
//}
//
//#ifdef _DEBUG
//void Test::TimedCall::DebugGui() {
//
//	ImGui::Begin("TimedCall");
//	ImGui::Text("time %d", time_);
//	ImGui::End();
//}
//#endif
