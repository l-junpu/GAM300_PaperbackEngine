#pragma once

/*
To Use:

	1. Create an Event:

		struct RandomEvent : paperback::event::instance
		{
			RandomEvent( paperback::component::entity Entity ) : m_Entity{ Entity } {}
			Paperback::EntityID m_Entity;
		};


	2. Create a public handler function within the System handling the Event

		void System::TestRandomEvent( std::shared_ptr< RandomEvent > Event )
		{
			DEBUG_PRINT("Random Event");

			// To access data ( Probably making a macro for this in the near future )
			static_cast< RandomEvent* >( *Event )->m_Entity;
		}


	3. Register Event inside OnCreate() - Subscribe Event Function has not been incorporated into Coordinator yet

		Action< std::shared_ptr< RandomEvent > > Event_1 = PPB_BIND( System::TestRandomEvent );
		m_Coordinator.SubscribeEvent( Event_1 );


	4. Publish Event

		m_Coordinator.PublishEvent< RandomEvent >( some_random_entity_component );
*/

namespace paperback::event
{
	struct instance;

	/****************************************************/
	/*!					 Event Handlers
	/****************************************************/
	class handler_interface
	{
	public:

		PPB_INLINE
		void Exec( std::shared_ptr<paperback::event::instance> Event ) noexcept;

	private:

		PPB_INLINE
		virtual void Function( std::shared_ptr<paperback::event::instance> Event ) noexcept = 0;
	};

	template < class EventType >
	class handler final : public handler_interface
	{
	public:

		PPB_INLINE
		handler( Action< std::shared_ptr<EventType> > Function ) noexcept;

		PPB_INLINE
		void Function( std::shared_ptr<paperback::event::instance> event ) noexcept override;

	private:

		Action< std::shared_ptr<EventType> > m_Function;
	};


	/****************************************************/
	/*!					   Event Bus
	/****************************************************/
	class dispatcher final
	{
		using EventList = std::vector< std::shared_ptr< handler_interface > >;
		using SubscriberMap = std::unordered_map< std::type_index, Ref< EventList > >;

	public:

		template < typename EventType, typename ...Args >
		void PublishEvent( Args&&... args ) noexcept;

		template <class EventType>
		void SubscribeEvent( Action< std::shared_ptr< EventType > > event ) noexcept;

	private:

		SubscriberMap m_Subscribers;
	};
}


















/*
// OLD CODE - To Possibly Incorporate
namespace paperback::event
{
	template < typename... T_ARGS >
	struct instance
	{
		struct info
		{
			using Callback = void( void*, T_ARGS... );
			Callback* 		 m_pCallback;
			void* 			 m_pClass;
		};

		template <auto T_FUNCTION, typename T_CLASS>
		constexpr void Register( T_CLASS& Class ) noexcept;

		PPB_INLINE
		void NotifyAll( T_ARGS... Args ) noexcept;

		std::vector<info>		m_Delegates;
	};
}
*/