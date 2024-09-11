#pragma once
#include <functional>

/// <summary>
/// 時限発動
/// </summary>
namespace Test {
	class TimedCall {
	public:

		TimedCall(std::function<void(void)> callback, const uint32_t& time);
		~TimedCall();

		/// <summary>
		/// 初期化関数
		/// </summary>
		void Init();

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update();

		/// <summary>
		/// 終了したか
		/// </summary>
		/// <returns></returns>
		const bool IsFinished() const { return isFinish_; }

#ifdef _DEBUG
		void DebugGui();
#endif //_DEBUG

	private:

		std::function<void(void)> callback_;

		uint32_t time_;

		bool isFinish_ = false;
	};

} // namespace