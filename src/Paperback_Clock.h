#pragma once

#include <ctime>
#include <ratio>
#include <chrono>
#include <memory>
#include <cstdint>
#include <utility>

namespace tools
{
	class clock
	{
	public:

		using HRClock = std::chrono::high_resolution_clock;
		using TimePoint = HRClock::time_point;
		using Duration = std::chrono::duration<float>;

		const float m_FixedDT;

		clock(float TimeScale = 1.0f);
		~clock() = default;

		uint32_t FPS();
		void Tick();
		float DeltaTime() const;
		auto Now() -> decltype(HRClock::now());
		float ComputeTime(TimePoint StartingTime);

		void TimeScale(const float& s = 1.0f);
		float TimeScale() const;

	private:

		float m_TimeElapsed;
		float m_TimeScale;
		Duration m_DT;
		TimePoint m_Start;
	};
}