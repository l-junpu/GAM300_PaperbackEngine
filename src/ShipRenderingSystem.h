#pragma once

struct ShipRenderingSystem : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "ShipRenderingSystem"
    };

    using query = std::tuple<
        paperback::query::none_of<Bullet>
    >;

    void operator()( Transform& transform, Timer& timer )
    {
        constexpr auto Scale = 3;
        glBegin(GL_QUADS);
        if (timer.m_Timer > 0.0f) glColor3f(1.0f, 1.0f, 1.0f);
        else                      glColor3f(0.0f, 1.0f, 0.0f);
        glVertex2i(transform.m_Position.m_X - Scale, transform.m_Position.m_Y - Scale);
        glVertex2i(transform.m_Position.m_X - Scale, transform.m_Position.m_Y + Scale);
        glVertex2i(transform.m_Position.m_X + Scale, transform.m_Position.m_Y + Scale);
        glVertex2i(transform.m_Position.m_X + Scale, transform.m_Position.m_Y - Scale);
        glEnd();
    }
};