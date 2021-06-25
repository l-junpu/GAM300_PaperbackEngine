#pragma once

namespace paperback
{
    namespace archetype
    {
        instance::instance( const tools::bits& ComponentBits ) noexcept :
            m_ComponentBits{ ComponentBits }
        { }

        void instance::Init( std::span<const component::info* const> Types ) noexcept
        {
            m_ComponentPool[m_PoolAllocationIndex].Init( paperback::settings::max_entities_per_pool_v, Types );
        }

        //template< typename T_CALLBACK >
        //uint32_t instance::CreateEntity( T_CALLBACK&& Function ) noexcept
        //{
        //    using func_traits = xcore::function::traits<T_CALLBACK>;

        //    return[&]< typename... T_COMPONENTS >(std::tuple<T_COMPONENTS...>*)
        //    {
        //        assert(m_ComponentBits.Has(component::info_v<T_COMPONENTS>.m_UID) && ...);

        //        // Generate the next valid ID within m_ComponentPool
        //        auto NewPoolIndex = m_ComponentPool[m_PoolAllocationIndex].Append();

        //        // Invoke lambda to assign data to Components
        //        Function
        //        (
        //            m_ComponentPool[m_PoolAllocationIndex].GetComponent< std::decay_t<T_COMPONENTS> >(
        //                NewPoolIndex,
        //                component::info_v<T_COMPONENTS>.m_UID
        //                ) ...
        //        );

        //        return NewPoolIndex;
        //    }(reinterpret_cast<func_traits::args_tuple*>(nullptr));
        //}

        //NEW TEST
        template< typename T_CALLBACK >
        instance::PoolDetails instance::CreateEntity( T_CALLBACK&& Function ) noexcept
        {
            using func_traits = xcore::function::traits<T_CALLBACK>;
        
            return[&]<typename... T_COMPONENTS >(std::tuple<T_COMPONENTS...>*)
            {
                assert( m_ComponentBits.Has( component::info_v<T_COMPONENTS>.m_UID ) && ... );
        
                // Generate the next valid ID within m_ComponentPool
                const auto PoolIndex = m_ComponentPool[ m_PoolAllocationIndex ].Append();

                if ( std::is_same_v<empty_lambda, T_CALLBACK> )
                {
                    // Update counters after checking processes??????????
                }
                else
                {
                    // Access guard here to initialize the object with data
                    Function( m_ComponentPool[ m_PoolAllocationIndex ].GetComponent<T_COMPONENTS>( PoolIndex ) ... );
                }
        
                return PoolDetails
                {
                    .m_Key = m_PoolAllocationIndex
                ,   .m_PoolIndex = PoolIndex
                };
            }( reinterpret_cast<func_traits::args_tuple*>( nullptr ) );
        }

        //uint32_t instance::DeleteEntity( const uint32_t pool_index ) noexcept // OLD
        //{
        //    return m_ComponentPool[m_PoolAllocationIndex].Delete( pool_index );
        //}

        uint32_t instance::DeleteEntity( const PoolDetails Details ) noexcept // NEW
        {
            return m_ComponentPool[ Details.m_Key ].Delete( Details.m_PoolIndex );
        }

        void instance::DestroyEntity( component::entity& Entity ) noexcept
        {
            assert( Entity.IsZombie() == false );
            Entity.m_Validation.m_bZombie = true;
            //++entity.m_Validation.m_Generation;
            m_DeleteList.push_back(Entity);
        }

        template < typename T_FUNCTION >
        requires(xcore::function::is_callable_v<T_FUNCTION>&&
            std::is_same_v<typename xcore::function::traits<T_FUNCTION>::return_type, void>&&
            xcore::function::traits<T_FUNCTION>::arg_count_v == 0)
        void instance::AccessGuard( T_FUNCTION&& Function ) noexcept
        {
            ++m_ProcessReference;
            Function();
            --m_ProcessReference;
            //if ( --m_ProcessReference == 0 ) UpdateStructuralChanges();
        }

        template < typename T > //OLD
        T& instance::GetComponent( const uint32_t& EntityIndex, const int& ComponentUID ) const noexcept
        {
            return m_ComponentPool[m_PoolAllocationIndex].GetComponent<T>(EntityIndex, ComponentUID);
        }

        template < typename T > //OLD
        T& instance::GetComponent( const uint32_t& EntityIndex ) const noexcept
        {
            return m_ComponentPool[m_PoolAllocationIndex].GetComponent<T>(EntityIndex, component::info_v<T>.m_UID);
        }

        // NEW
        template < typename T_COMPONENT >
        T_COMPONENT& instance::GetComponent( const PoolDetails Details ) const noexcept
        {
            return m_ComponentPool[ Details.m_Key ].GetComponent<T_COMPONENT>( Details.m_PoolIndex );
        }
    }

    namespace entity
    {
        //// OLD
        //void manager::RegisterEntity( const uint32_t pool_index, archetype::instance* archetype ) noexcept
        //{
        //    // Assign data to Entity Info
        //    m_EntityInfos[m_EntityIDTracker].m_PoolIndex = pool_index;
        //    m_EntityInfos[m_EntityIDTracker].m_pArchetype = archetype;

        //    // Modify the Global Index within EntityID Component
        //    auto& pEntityID = m_EntityInfos[m_EntityIDTracker].m_pArchetype->GetComponent<component::entity>(
        //        m_EntityInfos[m_EntityIDTracker].m_PoolIndex,
        //        component::info_v<component::entity>.m_UID
        //        );

        //    pEntityID.m_GlobalIndex = m_EntityIDTracker++;
        //}

        // NEW -- REPLACE ENTITYIDTRACKER WITH A LEGIT TRACKER (LINKED LIST)
        void manager::RegisterEntity(const PoolDetails Details, archetype::instance* archetype) noexcept
        {
            // Assign data to Entity Info
            m_EntityInfos[m_EntityIDTracker].m_PoolDetails = Details;
            m_EntityInfos[m_EntityIDTracker].m_pArchetype = archetype;

            // Modify the Global Index within EntityID Component
            auto& pEntityID = m_EntityInfos[m_EntityIDTracker].m_pArchetype->GetComponent<component::entity>(
                m_EntityInfos[m_EntityIDTracker].m_PoolDetails
                );

            pEntityID.m_GlobalIndex = m_EntityIDTracker++;
        }

        // LastIndex: Global Index of Last Entity within Archetype Pool
            // If Current Pool is Empty && Deleting Last Entity, LastIndex == -1
            // Else return GlobalIndex of the Last Entity for Re-mapping
        // DeletedIndex: Global Index of Deleted Entity
        void manager::RemoveEntity( const uint32_t LastIndex, const PoolDetails Details ) noexcept
        {
            if (LastIndex != paperback::settings::invalid_index_v)
                m_EntityInfos[LastIndex].m_PoolDetails = Details;
        }

        entity::info& manager::GetEntityInfo( const component::entity Entity ) const noexcept
        {
            return m_EntityInfos[ Entity.m_GlobalIndex ];
        }

        entity::info& manager::GetEntityInfo( const u32 GlobalIndex ) const noexcept
        {
            return m_EntityInfos[ GlobalIndex ];
        }

        archetype::instance& manager::GetArchetypeFromEntity( const uint32_t EntityID ) const noexcept
        {
            assert(EntityID < m_EntityIDTracker);

            return *(m_EntityInfos[EntityID].m_pArchetype);
        }

        void manager::FreeEntitiesInArchetype( archetype::instance* Archetype ) noexcept
        {
            if ( Archetype->m_ProcessReference == 0 && 
                 !Archetype->m_DeleteList.empty() )
            {
                for (auto& entity : Archetype->m_DeleteList)
                {
                    assert( entity.IsZombie() == true );

                    auto& Info = GetEntityInfo( entity );
                    //uint32_t PoolIndex = Info.m_PoolDetails;

                    RemoveEntity( Archetype->DeleteEntity( Info.m_PoolDetails ), Info.m_PoolDetails);
                }

                Archetype->m_DeleteList.clear();
            }
        }
    }
}