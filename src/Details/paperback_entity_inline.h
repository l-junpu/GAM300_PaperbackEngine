#pragma once

namespace paperback
{
    namespace archetype
    {
        instance::instance( coordinator::instance& Coordinator, const tools::bits& ComponentBits ) noexcept :
            m_Coordinator{ Coordinator },
            m_ComponentBits{ ComponentBits }
        { }

        void instance::Init( std::span<const component::info* const> Types ) noexcept
        {
            m_ComponentPool[m_PoolAllocationIndex].Init( Types );
            m_ComponentInfos = Types;
        }

        template< typename T_CALLBACK >
        instance::PoolDetails instance::CreateEntity( T_CALLBACK&& Function ) noexcept
        {
            using func_traits = xcore::function::traits<T_CALLBACK>;
        
            return[&]<typename... T_COMPONENTS>(std::tuple<T_COMPONENTS...>*)
            {
                assert( m_ComponentBits.Has( component::info_v<T_COMPONENTS>.m_UID ) && ... );
        
                // Generate the next valid ID within m_ComponentPool
                const auto PoolIndex = m_ComponentPool[ m_PoolAllocationIndex ].Append();

                if ( !std::is_same_v<empty_lambda, T_CALLBACK> )
                {
                    Function( m_ComponentPool[m_PoolAllocationIndex].GetComponent<T_COMPONENTS>(PoolIndex) ... );
                }
        
                return PoolDetails
                {
                    .m_Key = m_PoolAllocationIndex
                ,   .m_PoolIndex = PoolIndex
                };

            }( reinterpret_cast<func_traits::args_tuple*>( nullptr ) );
        }

        u32 instance::DeleteEntity( const PoolDetails Details ) noexcept
        {
            return m_ComponentPool[ Details.m_Key ].Delete( Details.m_PoolIndex );
        }

        void instance::DestroyEntity( component::entity& Entity ) noexcept
        {
            assert( Entity.IsZombie() == false );

            auto& EntityInfo = m_Coordinator.GetEntityInfo( Entity );
            auto& PoolEntity = GetComponent<component::entity>( EntityInfo.m_PoolDetails );

            assert( &Entity == &PoolEntity );

            Entity.m_Validation.m_bZombie
                = EntityInfo.m_Validation.m_bZombie
                = true;

            m_DeleteList.push_back( Entity );

            if ( m_ProcessReference == 0 ) UpdateStructuralChanges();
        }

        template < typename T_FUNCTION >
        requires( xcore::function::is_callable_v<T_FUNCTION>&&
                  std::is_same_v<typename xcore::function::traits<T_FUNCTION>::return_type, void>&&
                  xcore::function::traits<T_FUNCTION>::arg_count_v == 0 )
        void instance::AccessGuard( T_FUNCTION&& Function ) noexcept
        {
            ++m_ProcessReference;
            Function();
            if ( --m_ProcessReference == 0 ) UpdateStructuralChanges();
        }

        void instance::UpdateStructuralChanges() noexcept
        {
            if ( m_ProcessReference > 0 ) return;

            for (auto& Entity : m_DeleteList)
            {
                auto& Info = m_Coordinator.GetEntityInfo( Entity );
                m_Coordinator.RemoveEntity( DeleteEntity(Info.m_PoolDetails), Entity );
            }

            m_DeleteList.clear();
        }

        template < typename T_COMPONENT >
        T_COMPONENT& instance::GetComponent( const PoolDetails Details ) const noexcept
        {
            return m_ComponentPool[ Details.m_Key ].GetComponent<T_COMPONENT>( Details.m_PoolIndex );
        }

   //     template < concepts::ReferenceArgs... T_COMPONENTS >
   //     constexpr auto instance::GetComponentArray( vm::instance& Pool, u32 PoolIndex, std::tuple<T_COMPONENTS...>* ) const noexcept
   //     {
   //         using TupleComponents = std::tuple< T_COMPONENTS... >;
   //         using SortedTuple = component::details::sort_tuple_t< TupleComponents >;

   //         std::array<std::byte*, sizeof...(T_COMPONENTS)> ComponentArray;

   //         [&]<typename... SORTED_COMPONENTS>( std::tuple<SORTED_COMPONENTS...>* ) constexpr noexcept
   //         {
   //             (( ComponentArray[ xcore::types::tuple_t2i_v< SORTED_COMPONENTS, TupleComponents > ]
   //                  = [&]<typename T_COMPONENT>( std::tuple<T_COMPONENT>* ) constexpr noexcept
   //                    {
   //                        auto ComponentIndex = Pool.GetComponentIndexFromGUIDInSequence( component::info_v< SORTED_COMPONENTS >.m_Guid, 0 );
   //                        return Pool.m_ComponentPool[ ComponentIndex ][ component::info_v< T_COMPONENT >.m_Size * PoolIndex ];
   //                  
   //                    }( xcore::types::make_null_tuple_v< SORTED_COMPONENTS > )),
   //              ... );

   //         }( xcore::types::null_tuple_v< SortedTuple > );

   //         return ComponentArray;
   //     }

   //     template < concepts::MixedArgs... T_COMPONENTS >
   //     constexpr auto instance::GetComponentArray( vm::instance& Pool, u32 PoolIndex, std::tuple<T_COMPONENTS...>* ) const noexcept
   //     {
   //         using TupleComponents = std::tuple< T_COMPONENTS... >;
   //         using SortedTuple = component::details::sort_tuple_t< TupleComponents >;

   //         std::array<std::byte*, sizeof...(T_COMPONENTS)> ComponentArray;

   //         [&]<typename... SORTED_COMPONENTS>( std::tuple<SORTED_COMPONENTS...>* ) constexpr noexcept
			//{
			//	(( ComponentArray[ xcore::types::tuple_t2i_v< SORTED_COMPONENTS, TupleComponents > ]
   //                 = [&]<typename T_COMPONENT>( std::tuple<T_COMPONENT>* ) constexpr noexcept
			//	      {
			//	          int ComponentIndex = Pool.GetComponentIndexFromGUIDInSequence( component::info_v< SORTED_COMPONENTS >.m_Guid, 0 );

			//	          if constexpr ( std::is_pointer_v<T_COMPONENT> ) return ( ComponentIndex < 0 )
   //                                                                              ? nullptr 
   //                                                                              : Pool.m_ComponentPool[ ComponentIndex ][ sizeof(std::decay<T_COMPONENT>) * PoolIndex ];
			//	          else									          return   Pool.m_ComponentPool[ ComponentIndex ][ sizeof(std::decay<T_COMPONENT>) * PoolIndex ];

			//	      }( xcore::types::make_null_tuple_v< SORTED_COMPONENTS > )),
   //             ... );

			//}( xcore::types::null_tuple_v< SortedTuple > );
   //     }

        template < typename T_FUNCTION >
        component::entity& instance::TransferExistingEntity( component::entity& Entity, T_FUNCTION&& Function ) noexcept
        {
            using func_traits = xcore::function::traits<T_FUNCTION>;

            auto& EntityInfo = m_Coordinator.GetEntityInfo( Entity.m_GlobalIndex );
            const auto NewPoolIndex = m_ComponentPool[ m_PoolAllocationIndex ].TransferExistingComponents
            (
                EntityInfo.m_PoolDetails
            ,   EntityInfo.m_pArchetype.m_ComponentPool[ EntityInfo.m_PoolDetails.m_Key ]
            );

            EntityInfo.m_pArchetype = this;
            EntityInfo.m_PoolDetails.m_Key = m_PoolAllocationIndex;
            EntityInfo.m_PoolDetails.m_PoolIndex = NewPoolIndex;

            // TEMP
            [&]<typename... T_COMPONENTS>( std::tuple<T_COMPONENTS...>* )
            {
                assert( m_ComponentBits.Has( component::info_v<T_COMPONENTS>.m_UID ) && ... );
        
                // const auto PoolIndex = m_ComponentPool[ m_PoolAllocationIndex ].Append();

                if ( !std::is_same_v<empty_lambda, T_FUNCTION> )
                {
                    Function( m_ComponentPool[m_PoolAllocationIndex].GetComponent<T_COMPONENTS>(NewPoolIndex) ... );
                }

                // GOTTA RETURN ENTITY COMPONENT WITH UPDATED INDEX

            }( reinterpret_cast<func_traits::args_tuple*>( nullptr ) );
        }
    }

    namespace entity
    {
        void manager::RegisterEntity( const PoolDetails Details, archetype::instance& Archetype ) noexcept
        {
            u32   EntityGlobalIndex = AppendEntity();
            auto& EntityInfo        = GetEntityInfo( EntityGlobalIndex );

            EntityInfo = entity::info
                         {
                             .m_pArchetype  = &Archetype
                         ,   .m_PoolDetails = Details
                         ,   .m_Validation  = component::entity::Validation
                             {
                                 .m_Next    = 0
                             ,   .m_bZombie = false
                             }
                         };

            auto& Entity = EntityInfo.m_pArchetype->GetComponent<component::entity>( EntityInfo.m_PoolDetails );

            Entity.m_GlobalIndex          = EntityGlobalIndex;
            Entity.m_Validation.m_Next    = 0;
            Entity.m_Validation.m_bZombie = false;
        }

        template < typename T_FUNCTION >
        void manager::CreateEntity( T_FUNCTION&& Function, coordinator::instance& Coordinator ) noexcept
        {
            using func_traits = xcore::function::traits<T_FUNCTION>;

		    [&] <typename... T_COMPONENTS>(std::tuple<T_COMPONENTS...>*)
		    {
			    auto&      Archetype = GetOrCreateArchetype<T_COMPONENTS...>( Coordinator );
                const auto Details   = Archetype.CreateEntity(Function);

			    RegisterEntity( Details, Archetype );
		    }( reinterpret_cast<func_traits::args_tuple*>(nullptr) );
        }

        void manager::RemoveEntity( const u32 SwappedGlobalIndex, const component::entity DeletedEntity ) noexcept
        {
            if ( SwappedGlobalIndex != paperback::settings::invalid_index_v )
            {
                auto& Info = GetEntityInfo( DeletedEntity );
                m_EntityInfos[SwappedGlobalIndex].m_PoolDetails = Info.m_PoolDetails;
            }
            m_AvailableIndexes.push( DeletedEntity.m_GlobalIndex );
        }

        template < typename... T_COMPONENTS >
        archetype::instance& manager::GetOrCreateArchetype( coordinator::instance& Coordinator ) noexcept
        {
            static constexpr auto ComponentList = std::array{ &component::info_v<T_COMPONENTS>... };
            return GetOrCreateArchetype( ComponentList, Coordinator );
        }

        template < typename... T_COMPONENTS > // PRIVATE FN
        archetype::instance& manager::CreateArchetype( coordinator::instance& Coordinator, const tools::bits& Signature ) noexcept
        {
            m_pArchetypeList.push_back( std::make_unique<archetype::instance>( *this ) );
			m_ArchetypeBits.push_back( Signature );
            return *( m_pArchetypeList.back() );
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
            assert( EntityID < m_EntityIDTracker );

            return *( m_EntityInfos[EntityID].m_pArchetype );
        }

        void manager::FreeEntitiesInArchetype( archetype::instance* Archetype ) noexcept
        {
            if ( Archetype->m_ProcessReference == 0 && 
                 !Archetype->m_DeleteList.empty() )
            {
                for ( auto& Entity : Archetype->m_DeleteList )
                {
                    assert( Entity.IsZombie() == true );

                    auto& Info = GetEntityInfo( Entity );

                    RemoveEntity( Archetype->DeleteEntity( Info.m_PoolDetails ), Entity );
                }

                Archetype->m_DeleteList.clear();
            }
        }

        template < typename... T_COMPONENTS >
        std::vector<archetype::instance*> manager::Search() const noexcept
        {
            static constexpr auto ComponentList = std::array{ &component::info_v<T_COMPONENTS>... };
            return Search( ComponentList );
        }

        archetype::instance* manager::Search( const tools::bits& Bits ) const noexcept
        {
            for ( const auto& ArchetypeBits : m_ArchetypeBits )
            {
                if ( ArchetypeBits.Match( Bits ) )
                {
                    const auto index = static_cast<size_t>( &ArchetypeBits - &m_ArchetypeBits[0] );
                    return m_pArchetypeList[index].get();
                }
            }
            return nullptr;
        }

        std::vector<archetype::instance*> manager::Search( const tools::query& Query ) const noexcept
        {
            std::vector<archetype::instance*> ValidArchetypes;

            // Search for all Archetypes with valid Bit Signatures
            for ( const auto& ArchetypeBits : m_ArchetypeBits )
            {
                if ( Query.Compare( ArchetypeBits ) )
                {
                    const auto index = static_cast<size_t>( &ArchetypeBits - &m_ArchetypeBits[0] );
                    ValidArchetypes.push_back( m_pArchetypeList[index].get() );
                }
            }

            return ValidArchetypes;
        }


        // PRIVATE
        u32 manager::AppendEntity() noexcept
        {
            if (!m_AvailableIndexes.empty())
            {
                u32 GlobalIndex = m_AvailableIndexes.top();
                m_AvailableIndexes.pop();
                return GlobalIndex;
            }
            else
            {
                return m_EntityIDTracker++;
            }
        }

        archetype::instance& manager::GetOrCreateArchetype( std::span<const component::info* const> Types, coordinator::instance& Coordinator ) noexcept
        {
            tools::bits Query;

            for ( const auto& CInfo : Types )
                Query.Set(CInfo->m_UID);

            for ( auto& Archetype : m_ArchetypeBits )
            {
                if ( Archetype.Compare( Query ) )
                {
                    const auto index = static_cast<size_t>( &Archetype - &m_ArchetypeBits[0] );
                    return *( m_pArchetypeList[index] );
                }
            }

            m_pArchetypeList.push_back( std::make_unique<archetype::instance>(Coordinator, Query) );
            m_ArchetypeBits.push_back( Query );

            m_pArchetypeList.back()->Init( Types );

            return *( m_pArchetypeList.back() );
        }

        template < typename... T_COMPONENTS >
	    std::vector<archetype::instance*> manager::Search( std::span<const component::info* const> Types ) const noexcept
	    {
		    tools::bits Query;
		    std::vector<archetype::instance*> ValidArchetypes;

		    for ( const auto& cInfo : Types )
			    Query.Set( cInfo->m_UID );

		    for ( auto& Archetype : m_ArchetypeBits )
		    {
			    if ( Archetype.Compare( Query ) )
			    {
				    const auto Index = static_cast<size_t>( &Archetype - &m_ArchetypeBits[0] );
				    ValidArchetypes.push_back( m_pArchetypeList[Index].get() );
			    }
		    }

		    return ValidArchetypes;
	    }
    }
}