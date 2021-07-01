#pragma once

namespace paperback::system
{
	instance::instance( coordinator::instance& Coordinator ) noexcept : 
		m_Coordinator{ Coordinator }
	{ }
	
	void instance::Execute( void )
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
			USER_SYSTEM::Execute( );
		}
		else
		{
			tools::query Query;
			Query.AddQueryFromTuple( reinterpret_cast<typename USER_SYSTEM::query*>(nullptr) );
			Query.AddQueryFromFunction( *this );
			Coordinator.ForEach( Coordinator.Search(Query), *this );
		}
	}
	
	template < typename... T_SYSTEMS >
	constexpr void manager::RegisterSystems( coordinator::instance& Coordinator ) noexcept
	{
		( (RegisterSystem<T_SYSTEMS>( Coordinator )), ... );
	}

	template < typename T_SYSTEM >
	constexpr T_SYSTEM& manager::RegisterSystem( coordinator::instance& Coordinator_ ) noexcept
	{
		m_Systems.push_back
		(
			info
			{
				.m_System = std::make_unique< system::details::completed<T_SYSTEM> >( Coordinator_ ),
				.m_CallRun = []( system::instance& pSystem, coordinator::instance& Coordinator )
				{
					static_cast<system::details::completed<T_SYSTEM>&>(pSystem).Run( Coordinator );
				}
			}
		);
		return *( static_cast<T_SYSTEM*>( m_Systems.back().m_System.get()) );
	}

	void manager::Run( coordinator::instance& Coordinator ) noexcept
	{
		for ( const auto& System : m_Systems )
			System.m_CallRun( *System.m_System, Coordinator );
	}
}