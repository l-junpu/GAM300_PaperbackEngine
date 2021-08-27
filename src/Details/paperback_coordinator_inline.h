#pragma once

namespace paperback::coordinator
{
	instance::instance( void ) noexcept
	{
		m_CompMgr.RegisterComponent<paperback::component::entity>();
	}

	instance& instance::GetInstance() noexcept
	{
		static instance Instance;
		return Instance;
	}

	void instance::Update() noexcept
	{
		XCORE_PERF_FRAME_MARK()
		XCORE_PERF_FRAME_MARK_START("paperback::Frame")

		m_SystemMgr.Run( *this );

		XCORE_PERF_FRAME_MARK_END("paperback::Frame")
	}

	template < concepts::System... T_SYSTEMS >
	constexpr void instance::RegisterSystems() noexcept
	{
		m_SystemMgr.RegisterSystems<T_SYSTEMS...>( *this );
	}

	template< typename... T_COMPONENTS >
	constexpr void instance::RegisterComponents( void ) noexcept
	{
		m_CompMgr.RegisterComponents<T_COMPONENTS...>();
	}

	template < typename... T_COMPONENTS >
	archetype::instance& instance::GetOrCreateArchetype() noexcept
	{
		return m_EntityMgr.GetOrCreateArchetype<T_COMPONENTS...>( *this );
	}

	template< typename T_FUNCTION >
	void instance::CreateEntity( T_FUNCTION&& Function ) noexcept
	{
		m_EntityMgr.CreateEntity( Function, *this );
	}

	template< typename T_FUNCTION >
	void instance::CreateEntities( T_FUNCTION&& Function, const u32 Count ) noexcept
	{
		// To define
	}

	void instance::DeleteEntity( component::entity& Entity ) noexcept
	{
		assert( Entity.IsZombie() == false );
		auto& Info = GetEntityInfo( Entity );
		assert( Info.m_Validation == Entity.m_Validation );
		Info.m_pArchetype->DestroyEntity( Entity );
	}

	//TEMPORARY TEST ONLY (Try to remove)
	void instance::RemoveEntity( const uint32_t SwappedGlobalIndex, const component::entity Entity ) noexcept
	{
		m_EntityMgr.RemoveEntity( SwappedGlobalIndex, Entity );
	}

	template < concepts::TupleSpecialization T_TUPLE_ADD
			 , concepts::TupleSpecialization T_TUPLE_REMOVE
			 , concepts::Callable T_FUNCTION >
	component::entity AddOrRemoveComponents( component::entity Entity
										   , T_FUNCTION&& Function ) noexcept
	{
		if constexpr ( std::is_same_v< T_FUNCTION, empty_lambda > )
            return AddOrRemoveComponents
            ( Entity
            , component::sorted_info_array_v<T_TUPLE_ADD>
            , component::sorted_info_array_v<T_TUPLE_REMOVE>
            );
        else
            return AddOrRemoveComponents
            ( Entity
            , component::sorted_info_array_v<T_TUPLE_ADD>
            , component::sorted_info_array_v<T_TUPLE_REMOVE>
            , Function
            );
	}

	template < typename... T_COMPONENTS >
	std::vector<archetype::instance*> instance::Search() const noexcept
	{
		return m_EntityMgr.Search<T_COMPONENTS...>();
	}

	archetype::instance* instance::Search( const tools::bits& Bits ) const noexcept
	{
		return m_EntityMgr.Search( Bits );
	}
	
	std::vector<archetype::instance*> instance::Search( const tools::query& Query ) const noexcept
	{
		return m_EntityMgr.Search( Query );
	}

	template < concepts::Callable_Void T_FUNCTION>
	void instance::ForEach( const std::vector<archetype::instance*>& ArchetypeList, T_FUNCTION&& Function ) noexcept
	{
		using func_traits = xcore::function::traits<T_FUNCTION>;

		for (const auto& Archetype : ArchetypeList)
		{
			// Temporarily using only pool Index[0]
			auto& Pool = Archetype->m_ComponentPool[0];

			auto ComponentPtrs = [&]<typename... T_COMPONENTS>( std::tuple<T_COMPONENTS...>* ) constexpr noexcept
			{
				return std::array
				{
					[&]< typename T_C >( std::tuple<T_C>* ) constexpr noexcept
					{
						const auto I = Pool.GetComponentIndex(component::info_v<T_C>.m_UID);
						if constexpr (std::is_pointer_v<T_C>)	return (I < 0) ? nullptr : Pool.m_ComponentPool[I];
						else									return Pool.m_ComponentPool[I];
					}( xcore::types::make_null_tuple_v<T_COMPONENTS> )
						...
				};
			}( xcore::types::null_tuple_v< func_traits::args_tuple > );

			Archetype->AccessGuard([&]
			{
				for (int i = Pool.m_CurrentEntityCount; i; --i)
				{
					[&]< typename... T_COMPONENTS >( std::tuple<T_COMPONENTS...>* ) constexpr noexcept
					{
						//auto ComponentArray = Archetype->GetComponentArray(Pool, i, xcore::types::null_tuple_v< component_type_args::value >);

						Function( [&]<typename T_C>( std::tuple<T_C>* ) constexpr noexcept -> T_C
								  {
									  auto& pComponent = /*ComponentArray*/ComponentPtrs[xcore::types::tuple_t2i_v< T_C, typename func_traits::args_tuple >];
								  
									  if constexpr (std::is_pointer_v<T_C>) if (pComponent == nullptr) return reinterpret_cast<T_C>(nullptr);
								  
									  auto p = pComponent;
									  pComponent += sizeof(std::decay_t<T_C>);
								  
									  if constexpr (std::is_pointer_v<T_C>)		return reinterpret_cast<T_C>(p);
									  else										return reinterpret_cast<T_C>(*p);
								  
								  }( xcore::types::make_null_tuple_v<T_COMPONENTS> )
						... );

					}( xcore::types::null_tuple_v< func_traits::args_tuple > );
				}
			});
		}
	}

	template < concepts::Callable_Bool T_FUNCTION >
	void instance::ForEach( const std::vector<archetype::instance*>& ArchetypeList, T_FUNCTION&& Function ) noexcept
	{
		using func_traits = xcore::function::traits< T_FUNCTION >;

		for ( const auto& Archetype : ArchetypeList )
		{
			// Temporarily using only pool Index[0]
			auto& Pool = Archetype->m_ComponentPool[0];

			auto ComponentPtrs = [&]< typename... T_COMPONENTS >( std::tuple<T_COMPONENTS...>* ) constexpr noexcept
			{
				return std::array
				{
					[&]< typename T_C >( std::tuple<T_C>* ) constexpr noexcept
					{
						const auto I = Pool.GetComponentIndex( component::info_v<T_C>.m_UID );
						if constexpr ( std::is_pointer_v<T_C> )	return ( I < 0 ) ? nullptr : Pool.m_ComponentPool[I];
						else									return Pool.m_ComponentPool[I];
					}( xcore::types::make_null_tuple_v<T_COMPONENTS> )
						...
				};
			}( xcore::types::null_tuple_v< func_traits::args_tuple > );

			bool bBreak = false;

			Archetype->AccessGuard([&]
			{
				for (int i = Pool.m_CurrentEntityCount; i; --i)
				{
					if ( [&]<typename... T_COMPONENTS>( std::tuple<T_COMPONENTS...>* ) constexpr noexcept
					{
						return Function( [&]<typename T_C>(std::tuple<T_C>*) constexpr noexcept -> T_C
										 {
						                 	  auto& pComponent = ComponentPtrs[xcore::types::tuple_t2i_v< T_C, typename func_traits::args_tuple >];
						                 	  
						                 	  if constexpr (std::is_pointer_v<T_C>) if (pComponent == nullptr) return reinterpret_cast<T_C>(nullptr);
						                 	  
						                 	  auto p = pComponent;
						                 	  pComponent += sizeof(std::decay_t<T_C>);
						                 	  
						                 	  if constexpr (std::is_pointer_v<T_C>)		return reinterpret_cast<T_C>(p);
						                 	  else										return reinterpret_cast<T_C>(*p);

										 }( xcore::types::make_null_tuple_v<T_COMPONENTS> )
						... );
					}( xcore::types::null_tuple_v< func_traits::args_tuple > ) )
					{
						bBreak = true;
						break;
					}
				}
			});

			if (bBreak) break;
		}
	}

	entity::info& instance::GetEntityInfo( component::entity& Entity ) const noexcept
	{
		return m_EntityMgr.GetEntityInfo( Entity );
	}

	entity::info& instance::GetEntityInfo( const u32 GlobalIndex ) const noexcept
	{
		return m_EntityMgr.GetEntityInfo( GlobalIndex );
	}

	template< typename T_SYSTEM >
	T_SYSTEM* instance::FindSystem( void ) noexcept
	{
		return m_SystemMgr.FindSystem<T_SYSTEM>();
	}

	template< typename T_SYSTEM >
	T_SYSTEM& instance::GetSystem( void ) noexcept
	{
		auto p = m_SystemMgr.FindSystem<T_SYSTEM>();
		assert( p );
		return *p;
	}
	
	void instance::FreeEntitiesInArchetype( archetype::instance* Archetype ) noexcept
	{
		m_EntityMgr.FreeEntitiesInArchetype( Archetype );
	}

	// NEW PRIVATE FN
	template < concepts::Callable T_FUNCTION >
	component::entity instance::AddOrRemoveComponents ( const component::entity Entity, 
													    std::span<const component::info* const> Add,
													    std::span<const component::info* const> Remove,
													    T_FUNCTION&& Function ) noexcept
	{
		assert( Entity.IsZombie() == false );
		auto& EntityInfo             = GetEntityInfo( Entity.m_GlobalIndex );
		auto  UpdatedEntitySignature = EntityInfo.m_pArchetype->m_ComponentBits;
		assert( EntityInfo.m_Validation.m_bZombie == false );

		for ( const auto& CToAdd : Add )
		{
			assert( CToAdd.m_UID != 0 );
			UpdatedEntitySignature.Set( CToAdd.m_UID );
		}

		for ( const auto& CToRemove : Remove )
		{
			assert( CToRemove.m_UID != 0 );
			UpdatedEntitySignature.Remove( CToRemove.m_UID );
		}

		auto Archetype = Search( UpdatedEntitySignature );

		// Archetype already Exists
		if ( Archetype )
		{
			if ( std::is_same_v<T_FUNCTION, empty_lambda> ) Archetype->TransferExistingEntity( Entity );
			else											Archetype->TransferExistingEntity( Entity, Function );

			// Do sth with delete queue to remove the old entity's remaining components not affected by move ctor
		}
		// Create a new Archetype to fit
		else
		{
			int Count = 0;
			std::array<const component::info*, settings::max_components_per_entity_v > ComponentList;

			auto IsDupeInfo = [&]( const component::info* Info ) -> bool
			{
				for ( const auto& CInfo : ComponentList )
				{
					if ( CInfo.m_UID == Info->m_UID )
						return true;
				}
				return false;
			};

			for ( auto& CInfo : Archetype.m_ComponentInfos )
				ComponentList[Count++] = CInfo;
			for ( auto& CInfo : Add )
			{
				const auto Index = component::details::find_component_index_v( ComponentList, CInfo, Count );
				assert( Index > 0 );

				if ( ComponentList[ Count-1 ] == CInfo ) continue;

				if ( Index != Count )
					std::memmove( &ComponentList[ Index+1 ], &ComponentList[ Index ], (Count - Index) * sizeof(component::info*) );

				ComponentList[ Index ] = CInfo;
				++Count;
			}
			for ( auto& CInfo : Remove )
			{
				const auto Index = component::details::find_component_index_v(ComponentList, CInfo, Count);
				assert(Index > 0);

				if ( ComponentList[Index-1] == CInfo )
				{
					std::memmove( &ComponentList[ Index-1 ], &ComponentList[ Index ], (Count - Index) * sizeof(component::info*) );
					--Count;
				}
			}

			auto& Archetype = m_EntityMgr.CreateArchetype( *this, UpdatedEntitySignature );
			Archetype.Init( ComponentList );

			if constexpr ( std::is_same_v<T_FUNCTION, empty_lambda> ) return Archetype.TransferExistingEntity( Entity );
			else                                                      return Archetype.TransferExistingEntity( Entity, Function );
		}
	}

	float instance::DeltaTime() const noexcept
	{
		return m_Clock.DeltaTime();
	}

    void instance::SetTimeScale( const float s ) noexcept
	{
		m_Clock.TimeScale( s );
	}

	float instance::GetTimeScale() const noexcept
	{
		return m_Clock.TimeScale();
	}
}