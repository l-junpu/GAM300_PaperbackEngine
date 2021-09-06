#pragma once

struct Rigidbody
{
	constexpr static auto typedef_v = paperback::component::type::data
    {
        .m_pName = "Rigidbody"
    };

	xcore::vector2		m_Velocity;
	float				m_Mass;
};