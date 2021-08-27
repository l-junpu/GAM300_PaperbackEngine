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
                .m_Guid        = T_SYSTEM::typedef_v.m_Guid.isValid()
                                 ? T_SYSTEM::typedef_v.m_Guid
                                 : type::guid{ __FUNCSIG__ }
            ,   .m_TypeID      = T_SYSTEM::typedef_v.id_v
			,	.m_CallRun     = [](system::instance& pSystem, coordinator::instance& Coordinator)
							     {
								     static_cast<system::details::completed<T_SYSTEM>&>( pSystem ).Run( Coordinator );
							     }
			,	.m_pName		   = T_SYSTEM::typedef_v.m_pName
            };
        }
	}


	instance::instance( coordinator::instance& Coordinator ) noexcept : 
		m_Coordinator{ Coordinator }
	{ }
	
	void instance::Execute( coordinator::instance& Coordinator ) noexcept
	{ }
	
	template < typename USER_SYSTEM >
	details::completed<USER_SYSTEM>::completed( coordinator::instance& Coordinator ) noexcept :
		USER_SYSTEM{ Coordinator }
	{ }
	
	template < typename USER_SYSTEM >
	void details::completed<USER_SYSTEM>::Run( coordinator::instance& Coordinator ) noexcept
	{
		// If user defined their custom Execute
		if constexpr ( &USER_SYSTEM::Execute != &instance::Execute )
		{
			USER_SYSTEM::Execute( Coordinator );
		}
		else
		{
			tools::query Query;
			Query.AddQueryFromTuple( reinterpret_cast<typename USER_SYSTEM::query*>(nullptr) );
			Query.AddQueryFromFunction( *this );
			Coordinator.ForEach( Coordinator.Search(Query), *this );
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
		auto* pSystem = m_Systems.back().second.get();
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
		m_SystemClock.Tick();
		for ( const auto& [ Info, System ] : m_Systems )
			Info->m_CallRun( *System, Coordinator );
	}
}