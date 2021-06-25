#pragma once

namespace paperback::coordinator
{
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
		static constexpr auto ComponentList = std::array{ &component::info_v<T_COMPONENTS>... };
		return GetOrCreateArchetype( ComponentList );
	}

	template< typename T_FUNCTION >
	void instance::CreateEntity( T_FUNCTION&& Function ) noexcept
	{
		using func_traits = xcore::function::traits<T_FUNCTION>;

		[&] < typename... T_COMPONENTS >(std::tuple<T_COMPONENTS...>*)
		{
			auto& Archetype = GetOrCreateArchetype<T_COMPONENTS...>();
			m_EntityMgr.RegisterEntity( Archetype.CreateEntity(Function), &Archetype );
		}(reinterpret_cast<func_traits::args_tuple*>(nullptr));
	}

	template< typename T_FUNCTION >
	void instance::CreateEntities( T_FUNCTION&& Function, const u32 Count ) noexcept
	{
		
	}

	void instance::DeleteEntity( component::entity& Entity ) noexcept
	{
		assert( Entity.IsZombie() == false );
		auto& Info = GetEntityInfo( Entity );
		Info.m_pArchetype->DestroyEntity( Entity );
	}

	template < typename... T_COMPONENTS >
	std::vector<archetype::instance*> instance::Search() const noexcept
	{
		static constexpr auto ComponentList = std::array{ &component::info_v<T_COMPONENTS>... };
		return Search( ComponentList );
	}
	
	std::vector<archetype::instance*> instance::Search( const tools::query& Query ) const noexcept
	{
		std::vector<archetype::instance*> ValidArchetypes{};
	
		// Search for all Archetypes with valid Bit Signatures
		for (const auto& ArchetypeBits : m_ArchetypeBits)
		{
			if (Query.Compare(ArchetypeBits))
			{
				const auto index = static_cast<size_t>(&ArchetypeBits - &m_ArchetypeBits[0]);
				ValidArchetypes.push_back(m_pArchetypeList[index].get());
			}
		}
	
		return ValidArchetypes;
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

			// Generates an Array of m_UID's to relevant Components
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

						}(xcore::types::make_null_tuple_v<T_COMPONENTS>)
							...);
					}(xcore::types::null_tuple_v<func_traits::args_tuple>);
				}
			});

			FreeEntitiesInArchetype(Archetype);
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

			// Generates an Array of m_UID's to relevant Components
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
					if ( [&]<typename... T_COMPONENTS>(std::tuple<T_COMPONENTS...>*) constexpr noexcept
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

			FreeEntitiesInArchetype( Archetype );
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
		m_EntityMgr.FreeEntitiesInArchetype(Archetype);
	}

	//
	// PRIVATE FUNCTIONS
	//
	archetype::instance& instance::GetOrCreateArchetype(std::span<const component::info* const> Types) noexcept
	{
		// Set Component Bits
		tools::bits Query{};

		for (const auto& CInfo : Types)
			Query.Set(CInfo->m_UID);

		// Search for all Archetypes with valid Bit Signatures
		for (auto& Archetype : m_ArchetypeBits)
		{
			if (Archetype.Compare(Query))
			{
				const auto index = static_cast<size_t>(&Archetype - &m_ArchetypeBits[0]);
				return *(m_pArchetypeList[index]);
			}
		}

		m_pArchetypeList.push_back(std::make_unique<archetype::instance>(Query));
		m_ArchetypeBits.push_back(Query);

		m_pArchetypeList.back()->Init(Types);

		return *(m_pArchetypeList.back());
	}

	template < typename... T_COMPONENTS >
	std::vector<archetype::instance*> instance::Search(std::span<const component::info* const> Types) const noexcept
	{
		tools::bits Query;
		std::vector<archetype::instance*> ValidArchetypes{};

		for (const auto& cInfo : Types)
			Query.Set(cInfo->m_UID);

		for (auto& Archetype : m_ArchetypeBits)
		{
			if (Archetype.Compare(Query))
			{
				const auto Index = static_cast<size_t>(&Archetype - &m_ArchetypeBits[0]);
				ValidArchetypes.push_back(m_pArchetypeList[Index].get());
			}
		}

		return ValidArchetypes;
	}
}