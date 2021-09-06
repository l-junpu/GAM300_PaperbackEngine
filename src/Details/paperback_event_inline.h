#pragma once

namespace paperback::event
{
	/****************************************************/
	/*!					 Event Handlers
	/****************************************************/
	void handler_interface::Exec( std::shared_ptr<paperback::event::instance> Event ) noexcept
	{
		Function( Event );
	}

	template < class EventType >
	handler<EventType>::handler( Action< std::shared_ptr<EventType> > Function ) noexcept :
		m_Function{ Function }
	{ }

	template < class EventType >
	void handler<EventType>::Function( std::shared_ptr<paperback::event::instance> event ) noexcept
	{
		m_Function( std::static_pointer_cast< EventType >( event ) );
	}


	/****************************************************/
	/*!					   Event Bus
	/****************************************************/
	template < typename EventType, typename ...Args >
	void dispatcher::PublishEvent( Args&&... args ) noexcept
	{
		auto it = m_Subscribers.find( typeid(EventType) );

		// Replace this assertion with a log instead and return
		PPB_ASSERT_MSG( it == m_Subscribers.end(),
						"EventBus<EventType>::PublishEvent() -> EventType does not exist" );

		std::shared_ptr<EventList> EventsList = it->second;
		std::shared_ptr<EventType> EventDetails = std::make_shared< EventType >( args... );

		for ( auto& SubscribedEvent : *EventsList )
		{
			SubscribedEvent->Exec( EventDetails );
		}
	}

	template <class EventType>
	void dispatcher::SubscribeEvent( Action< std::shared_ptr< EventType > > event ) noexcept
	{
		auto it = m_Subscribers.find( typeid( EventType ) );

		if ( it == m_Subscribers.end() )
		{
			auto list = CreateRef<EventList>();
			m_Subscribers[ typeid(EventType) ] = list;
		}

		m_Subscribers[ typeid(EventType) ]->push_back( std::make_shared< EventHandler< EventType > >( event ) );
	}
}
















/*
// OLD CODE - To Possibly Incorporate
namespace paperback::event
{
	template < typename... T_ARGS >
	template < auto T_FUNCTION, typename T_CLASS >
	constexpr void instance<T_ARGS...>::Register( T_CLASS& Class ) noexcept
	{
		m_Delegates.push_back
		(
			info
			{
				.m_pCallback = []( void* pPtr, T_ARGS... Args )
				{
					std::invoke( T_FUNCTION, reinterpret_cast<T_CLASS*>( pPtr), std::forward<T_ARGS>( Args )... );
				},
				.m_pClass = Class
			}
		);
	}

	template < typename... T_ARGS >
	void instance<T_ARGS...>::NotifyAll( T_ARGS... Args ) noexcept
	{
		for ( auto& E : m_Delegates )
		{
			E.m_pCallback( E.m_pClass, std::forward<T_ARGS>( Args )... );
		}
	}
}
*/