#pragma once

#include "Bullet.h"
#include "Timer.h"
#include "Transform.h"

struct BulletLogicSystem : paperback::system::instance
{
    void operator()(paperback::component::Entity& entity, Transform& transform, Timer& timer, Bullet& bullet) const noexcept
    {
        if (entity.IsZombie()) return;

        timer.m_Timer -= 0.01f;
        if (timer.m_Timer <= 0.0f)
        {
            m_Coordinator.DeleteEntity(entity);
            return;
        }

        // Check collisions
        tools::query Query;
        Query.m_Must.AddFromComponents<Transform>();

        m_Coordinator.ForEach( m_Coordinator.Search(Query), [&]( paperback::component::Entity& e, Transform& xform ) noexcept -> bool
        {
            assert( entity.IsZombie() == false );

            // Do not check against self
            if ( ( &entity == &e ) || ( e.IsZombie() ) || ( bullet.m_Owner.m_GlobalIndex == e.m_GlobalIndex ) ) return false;

            constexpr auto min_distance_v = 4;
            if ((transform.m_Position - xform.m_Position).getLengthSquared() < min_distance_v * min_distance_v)
            {
                m_Coordinator.DeleteEntity(entity);
                m_Coordinator.DeleteEntity(e);
                return true;
            }
            return false;
        });
    }
};