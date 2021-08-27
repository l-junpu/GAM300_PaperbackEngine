#pragma once

#include "Transform.h"
#include "Rigidbody.h"

struct PhysicsSystem : paperback::system::instance
{
    void operator()( Transform& transform, Rigidbody& rigidbody )
    {
        transform.m_Position += rigidbody.m_Velocity * m_Coordinator.DeltaTime();

        // X-Out-Of-Bounds
        if (transform.m_Position.m_X < 0.0f)
        {
            transform.m_Position.m_X = 0.0f;
            rigidbody.m_Velocity.m_X = -rigidbody.m_Velocity.m_X;
        }
        else if (transform.m_Position.m_X >= m_Engine.m_Width)
        {
            transform.m_Position.m_X = m_Engine.m_Width;
            rigidbody.m_Velocity.m_X = -rigidbody.m_Velocity.m_X;
        }

        // Y-Out-Of-Bounds
        if (transform.m_Position.m_Y < 0.0f)
        {
            transform.m_Position.m_Y = 0.0f;
            rigidbody.m_Velocity.m_Y = -rigidbody.m_Velocity.m_Y;
        }
        else if (transform.m_Position.m_Y >= m_Engine.m_Height)
        {
            transform.m_Position.m_Y = m_Engine.m_Height;
            rigidbody.m_Velocity.m_Y = -rigidbody.m_Velocity.m_Y;
        }
    }
};