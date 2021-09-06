#pragma once

namespace paperback::system
{
	namespace details
    {
        template< typename T_SYSTEM >
        consteval system::type::info CreateInfo( void ) noexcept
        {
            return system::type::info
            {
                .m_Guid          = T_SYSTEM::typedef_v.m_Guid.isValid()
                                   ? T_SYSTEM::typedef_v.m_Guid
                                   : type::guid{ __FUNCSIG__ }
            ,   .m_TypeID        = T_SYSTEM::typedef_v.id_v
			,	.m_RunSystem   = []( coordinator::instance& Coordinator, system::instance& pSystem, system::type::call UpdateType )
							     {
								     static_cast<system::details::completed<T_SYSTEM>&>( pSystem ).Run( Coordinator, UpdateType );
							     }
			,	.m_pName		   = T_SYSTEM::typedef_v.m_pName
            };
        }
	}



	instance::instance( coordinator::instance& Coordinator ) noexcept : 
		m_Coordinator{ Coordinator }
	{ }
	
	//void instance::Execute( coordinator::instance& Coordinator ) noexcept
	//{ }
	
	template < typename USER_SYSTEM >
	details::completed<USER_SYSTEM>::completed( coordinator::instance& Coordinator ) noexcept :
		USER_SYSTEM{ Coordinator }
	{ }
	
	template < typename USER_SYSTEM >
	void details::completed<USER_SYSTEM>::Run( coordinator::instance& Coordinator, const paperback::system::type::call Type ) noexcept
	{
		switch ( Type )
		{
			case system::type::call::CREATED:
			{
				if constexpr ( &USER_SYSTEM::OnSystemCreated != &system_interface::OnSystemCreated )
				{
					USER_SYSTEM::OnSystemCreated();
				}
				break;
			}
			case system::type::call::FRAME_START:
			{
				if constexpr ( &USER_SYSTEM::OnFrameStart != &system_interface::OnFrameStart )
				{
					USER_SYSTEM::OnFrameStart();
				}
				break;
			}
			case system::type::call::PRE_UPDATE:
			{
				if constexpr ( &USER_SYSTEM::PreUpdate != &system_interface::PreUpdate )
				{
					USER_SYSTEM::PreUpdate();
				}
				break;
			}
			case system::type::call::UPDATE:
			{
				if constexpr ( &USER_SYSTEM::Update != &system_interface::Update )
				{
					USER_SYSTEM::Update();
				}
				else
				{
					tools::query Query;
					Query.AddQueryFromTuple( reinterpret_cast<typename USER_SYSTEM::query*>(nullptr) );
					Query.AddQueryFromFunction( *this );
					Coordinator.ForEach( Coordinator.Search(Query), *this );
				}
				break;
			}
			case system::type::call::POST_UPDATE:
			{
				if constexpr ( &USER_SYSTEM::PostUpdate != &system_interface::PostUpdate )
				{
					USER_SYSTEM::PostUpdate();
				}
				break;
			}
			case system::type::call::FRAME_END:
			{
				if constexpr ( &USER_SYSTEM::OnFrameEnd != &system_interface::OnFrameEnd )
				{
					USER_SYSTEM::OnFrameEnd();
				}
				break;
			}
		}
	}



	manager::manager( tools::clock& Clock ) :
		m_SystemClock{ Clock }
	{ }
	
	template < typename... T_SYSTEMS >
	constexpr void manager::RegisterSystems( coordinator::instance& Coordinator ) noexcept
	{
		( (RegisterSystem<T_SYSTEMS>( Coordinator )), ... );
	}

	template < typename T_SYSTEM >
	constexpr T_SYSTEM& manager::RegisterSystem( coordinator::instance& Coordinator ) noexcept
	{
		m_Systems.push_back({ &system::info_v<T_SYSTEM>, std::make_unique< system::details::completed<T_SYSTEM> >(Coordinator) });
		auto* pInfo   = m_Systems.back().first;
		auto* pSystem = m_Systems.back().second.get();
		pInfo->m_RunSystem( Coordinator, *pSystem, system::type::call::CREATED );
		m_SystemMap.emplace( std::make_pair( system::info_v<T_SYSTEM>.m_Guid, pSystem ) );

		return *( static_cast<T_SYSTEM*>( pSystem ) );
	}

	template < typename T_SYSTEM >
	T_SYSTEM* manager::FindSystem( void ) noexcept
	{
		return m_SystemMap.find( system::info_v<T_SYSTEM>.m_Guid ) != m_SystemMap.end()
			   ? m_SystemMap.find( system::info_v<T_SYSTEM>.m_Guid )->second
			   : nullptr;
	}

	void manager::Run( coordinator::instance& Coordinator ) noexcept
	{
		// Track Frame Time
		m_SystemClock.Tick();

		for ( const auto& [ Info, System ] : m_Systems )
			Info->m_RunSystem( Coordinator, *System, system::type::call::FRAME_START );

		for ( const auto& [ Info, System ] : m_Systems )
			Info->m_RunSystem( Coordinator, *System, system::type::call::PRE_UPDATE );

		for ( const auto& [ Info, System ] : m_Systems )
			Info->m_RunSystem( Coordinator, *System, system::type::call::UPDATE );

		for ( const auto& [ Info, System ] : m_Systems )
			Info->m_RunSystem( Coordinator, *System, system::type::call::POST_UPDATE );

		for ( const auto& [ Info, System ] : m_Systems )
			Info->m_RunSystem( Coordinator, *System, system::type::call::FRAME_END );
	}
}