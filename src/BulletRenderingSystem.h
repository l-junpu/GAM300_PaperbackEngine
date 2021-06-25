#pragma once

#include "Transform.h"
#include "Rigidbody.h"

struct BulletRenderingSystem : paperback::system::instance
{
    using query = std::tuple<
        paperback::query::must<Bullet>
    >;

    void operator()( Transform& transform, Rigidbody& rigidbody )
    {
        constexpr auto ScaleX = 1;
        constexpr auto ScaleY = 3;
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2i(transform.m_Position.m_X + rigidbody.m_Velocity.m_X * ScaleY, transform.m_Position.m_Y + rigidbody.m_Velocity.m_Y * ScaleY);
        glVertex2i(transform.m_Position.m_X + rigidbody.m_Velocity.m_Y * ScaleX, transform.m_Position.m_Y - rigidbody.m_Velocity.m_X * ScaleX);
        glVertex2i(transform.m_Position.m_X - rigidbody.m_Velocity.m_Y * ScaleX, transform.m_Position.m_Y + rigidbody.m_Velocity.m_X * ScaleX);
        glEnd();
    }
};