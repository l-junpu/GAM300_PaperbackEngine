#pragma once

struct Transform
{
	constexpr static auto typedef_v = paperback::component::type::data
	{
		.m_pName = "Transform"
	};

	xcore::vector2	 m_Offset;
	xcore::vector2	 m_Position;
	xcore::vector2	 m_Rotation;
	xcore::vector2	 m_LocalScale;
};