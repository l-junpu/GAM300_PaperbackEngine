#pragma once

struct Timer
{
    constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Timer"
    };

	float m_Timer = 0.0f;
};