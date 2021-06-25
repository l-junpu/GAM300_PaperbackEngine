#pragma once

#include "Timer.h"
#include "Transform.h"
#include "Bullet.h"
#include "Rigidbody.h"

struct ShipLogicSystem : paperback::system::instance
{
    using query = std::tuple<
        paperback::query::none_of<Bullet>
    >;

    void operator()( paperback::component::entity& Entity, Transform& transform, Timer& timer ) const noexcept
    {
        if (timer.m_Timer > 0.0f)
        {
            timer.m_Timer -= 0.01f;
            return;
        }

        tools::query Query;
        Query.m_NoneOf.AddFromComponents<Bullet>();

        m_Coordinator.ForEach( m_Coordinator.Search(Query), [&]( Transform& xform ) noexcept -> bool
        {
            if (&transform == &xform) return false;

            auto Direction = xform.m_Position - transform.m_Position;
            const auto DistanceSq = Direction.getLengthSquared();

            constexpr auto min_distance_v = 30;

            if (DistanceSq < min_distance_v * min_distance_v)
            {
                timer.m_Timer = 5;

                m_Engine.m_Coordinator.CreateEntity([&]( paperback::component::entity& Bullet_Entity, Transform& xform, Rigidbody& rb, Timer& timer, Bullet& bullet)
                {
                    Direction /= std::sqrt(DistanceSq);
                    xform.m_Position = transform.m_Position + rb.m_Velocity;
                    rb.m_Velocity = Direction * 3.0f;

                    bullet.m_Owner = Entity;
                    timer.m_Timer = 3.0f;
                });

                return true;
            }
            return false;
        });
    }
};