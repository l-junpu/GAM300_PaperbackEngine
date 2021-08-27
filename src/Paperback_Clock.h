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

        clock( float TimeScale = 1.0f ) noexcept;
        ~clock() = default;

        uint32_t FPS() noexcept;
        void     Tick() noexcept;
        float    DeltaTime() const noexcept;
        auto     Now() noexcept -> decltype( HRClock::now() );
        float    ComputeTime( TimePoint StartingTime ) noexcept;

        void     TimeScale( const float s = 1.0f ) noexcept;
        float    TimeScale() const noexcept;

    private:

        const float   m_FixedDT;
        float         m_TimeElapsed;
        float         m_TimeScale;
        Duration      m_DT;
        TimePoint     m_Start;
    };
}