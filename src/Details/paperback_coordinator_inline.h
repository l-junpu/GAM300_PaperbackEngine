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

	template < typename... T_SYSTEMS >
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

	template < typename... T_COMPONENTS >
	std::vector<archetype::instance*> instance::Search() const noexcept
	{
		return m_EntityMgr.Search<T_COMPONENTS...>();
	}
	
	std::vector<archetype::instance*> instance::Search( const tools::query& Query ) const noexcept
	{
		return m_EntityMgr.Search( Query );
	}

	template < typename T_FUNCTION>
	requires(xcore::function::is_callable_v<T_FUNCTION>&&
		std::is_same_v< void, typename xcore::function::traits<T_FUNCTION>::return_type >)
	void instance::ForEach( const std::vector<archetype::instance*>& ArchetypeList, T_FUNCTION&& Function ) noexcept
	{
		using func_traits = xcore::function::traits<T_FUNCTION>;

		for (const auto& Archetype : ArchetypeList)
		{
			// Temporarily using only pool Index[0]
			auto& Pool = Archetype->m_ComponentPool[0];

			auto ComponentPtrs = [&]<typename... T_COMPONENTS>(std::tuple<T_COMPONENTS...>*) constexpr noexcept
			{
				return std::array
				{
					[&] <typename T_C>(std::tuple<T_C>*) constexpr noexcept
					{
						const auto I = Pool.GetComponentIndex(component::info_v<T_C>.m_UID);
						if constexpr (std::is_pointer_v<T_C>)	return (I < 0) ? nullptr : Pool.m_ComponentPool[I];
						else									return Pool.m_ComponentPool[I];
					}(xcore::types::make_null_tuple_v<T_COMPONENTS>)
						...
				};
			}(xcore::types::null_tuple_v<func_traits::args_tuple>);

			Archetype->AccessGuard([&]
			{
				for (int i = Pool.m_CurrentEntityCount; i; --i)
				{
					[&]<typename... T_COMPONENTS>(std::tuple<T_COMPONENTS...>*) constexpr noexcept
					{
						Function([&]<typename T_C>(std::tuple<T_C>*) constexpr noexcept -> T_C
						{
							auto& pComponent = ComponentPtrs[xcore::types::tuple_t2i_v<T_C, typename func_traits::args_tuple>];

							if constexpr (std::is_pointer_v<T_C>) if (pComponent == nullptr) return reinterpret_cast<T_C>(nullptr);

							auto p = pComponent;								// Back up the pointer
							pComponent += sizeof(std::decay_t<T_C>);			// Get ready for the next entity

							if constexpr (std::is_pointer_v<T_C>)		return reinterpret_cast<T_C>(p);
							else										return reinterpret_cast<T_C>(*p);

						}( xcore::types::make_null_tuple_v<T_COMPONENTS> )
							... );
					}( xcore::types::null_tuple_v<func_traits::args_tuple> );
				}
			});
		}
	}

	template < typename T_FUNCTION>
	requires( xcore::function::is_callable_v<T_FUNCTION> &&
			  std::is_same_v< bool, typename xcore::function::traits<T_FUNCTION>::return_type > )
	void instance::ForEach( const std::vector<archetype::instance*>& ArchetypeList, T_FUNCTION&& Function ) noexcept
	{
		using func_traits = xcore::function::traits<T_FUNCTION>;

		for (const auto& Archetype : ArchetypeList)
		{
			// Temporarily using only pool Index[0]
			auto& Pool = Archetype->m_ComponentPool[0];

			auto ComponentPtrs = [&]<typename... T_COMPONENTS>(std::tuple<T_COMPONENTS...>*) constexpr noexcept
			{
				return std::array
				{
					[&]<typename T_C>(std::tuple<T_C>*) constexpr noexcept
					{
						const auto I = Pool.GetComponentIndex( component::info_v<T_C>.m_UID );
						if constexpr (std::is_pointer_v<T_C>)	return (I < 0) ? nullptr : Pool.m_ComponentPool[I];
						else									return Pool.m_ComponentPool[I];
					}( xcore::types::make_null_tuple_v<T_COMPONENTS> )
						...
				};
			}( xcore::types::null_tuple_v<func_traits::args_tuple> );

			bool bBreak = false;

			Archetype->AccessGuard([&]
			{
				for (int i = Pool.m_CurrentEntityCount; i; --i)
				{
					if ([&]<typename... T_COMPONENTS>(std::tuple<T_COMPONENTS...>*) constexpr noexcept
					{
						return Function([&]<typename T_C>(std::tuple<T_C>*) constexpr noexcept -> T_C
						{
							auto& pComponent = ComponentPtrs[xcore::types::tuple_t2i_v<T_C, typename func_traits::args_tuple>];

							if constexpr (std::is_pointer_v<T_C>) if (pComponent == nullptr) return reinterpret_cast<T_C>(nullptr);

							auto p = pComponent;								// Back up the pointer
							pComponent += sizeof(std::decay_t<T_C>);			// Get ready for the next entity

							if constexpr (std::is_pointer_v<T_C>)		return reinterpret_cast<T_C>(p);
							else										return reinterpret_cast<T_C>(*p);

						}( xcore::types::make_null_tuple_v<T_COMPONENTS> )
						... );
					}( xcore::types::null_tuple_v<func_traits::args_tuple> ) )
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
	
	void instance::FreeEntitiesInArchetype( archetype::instance* Archetype ) noexcept
	{
		m_EntityMgr.FreeEntitiesInArchetype( Archetype );
	}
}