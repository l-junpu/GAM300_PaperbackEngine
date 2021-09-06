#pragma once

struct BulletRenderingSystem : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "BulletRenderingSystem"
    };

    using query = std::tuple<
        paperback::query::must<Bullet>
    >;

    void operator()( Transform& transform, Rigidbody& rigidbody )
    {
        constexpr auto ScaleX = 1 / 24.0f;
        constexpr auto ScaleY = 3 / 24.0f;
        glBegin(GL_TRIANGLES);
        glColor3f(1.0f, 0.0f, 0.0f);
        glVertex2i(transform.m_Position.m_X + rigidbody.m_Velocity.m_X * ScaleY, transform.m_Position.m_Y + rigidbody.m_Velocity.m_Y * ScaleY);
        glVertex2i(transform.m_Position.m_X + rigidbody.m_Velocity.m_Y * ScaleX, transform.m_Position.m_Y - rigidbody.m_Velocity.m_X * ScaleX);
        glVertex2i(transform.m_Position.m_X - rigidbody.m_Velocity.m_Y * ScaleX, transform.m_Position.m_Y + rigidbody.m_Velocity.m_X * ScaleX);
        glEnd();
    }
};