#pragma once

struct BulletLogicSystem : paperback::system::instance
{
    constexpr static auto typedef_v = paperback::system::type::update
    {
        .m_pName = "BulletLogicSystem"
    };

    void operator()( paperback::component::entity& Entity, Transform& transform, Timer& timer, Bullet& bullet ) const noexcept
    {
        if (Entity.IsZombie()) return;

        timer.m_Timer -= m_Coordinator.DeltaTime();
        if (timer.m_Timer <= 0.0f)
        {
            m_Coordinator.DeleteEntity(Entity);
            return;
        }

        // Check collisions
        tools::query Query;
        Query.m_Must.AddFromComponents<Transform>();

        m_Coordinator.ForEach( m_Coordinator.Search(Query), [&]( paperback::component::entity& Dynamic_Entity, Transform& xform ) noexcept -> bool
        {
            assert(Entity.IsZombie() == false );

            // Do not check against self
            if ( ( &Entity == &Dynamic_Entity) || ( Dynamic_Entity.IsZombie() ) || ( bullet.m_Owner.m_GlobalIndex == Dynamic_Entity.m_GlobalIndex ) ) return false;

            constexpr auto min_distance_v = 4;
            if ((transform.m_Position - xform.m_Position).getLengthSquared() < min_distance_v * min_distance_v)
            {
                m_Coordinator.DeleteEntity( Entity );
                m_Coordinator.DeleteEntity( Dynamic_Entity );
                return true;
            }
            return false;
        });
    }
};