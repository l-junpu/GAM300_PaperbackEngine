#include "Paperback_Clock.h"

namespace tools
{
	clock::clock(float TimeScale) :
		m_FixedDT{ 1 / 60.0f },
		m_TimeScale{ TimeScale },
		m_DT{ },
		m_Start{ Now() }
	{ }

	uint32_t clock::FPS()
	{
		return static_cast<uint32_t>(1.0f / m_DT.count());
	}

	void clock::Tick()
	{
		m_DT = HRClock::now() - m_Start;
		m_Start = HRClock::now();

		m_TimeElapsed += m_DT.count();
	}

	float clock::DeltaTime() const
	{
		return m_DT.count();
	}

	auto clock::Now() -> decltype(HRClock::now())
	{
		return HRClock::now();
	}

	float clock::ComputeTime(TimePoint StartingTime)
	{
		return static_cast<Duration>(HRClock::now() - StartingTime).count();
	}

	void clock::TimeScale(const float& TimeScale)
	{
		m_TimeScale = TimeScale;
	}

	float clock::TimeScale() const
	{
		return m_TimeScale;
	}
}