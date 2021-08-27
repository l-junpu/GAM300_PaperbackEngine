#pragma once

namespace paperback::system
{
	namespace type
	{
		using guid = xcore::guid::unit<64, struct system_tag>;

		enum class id : u8
		{
			UPDATE = 0
		};

		struct update
		{
			static constexpr auto id_v = id::UPDATE;

            type::guid                   m_Guid { };
            const char*                  m_pName{ };
		};

		struct info
		{
			using call_run = void( system::instance&, coordinator::instance& );

			const type::guid                                m_Guid;
			const type::id                                  m_TypeID;

			//std::unique_ptr<paperback::system::instance>	m_System;
			call_run*										m_CallRun;

			const char*										m_pName{ };
		};
	}

	namespace details
    {
        template< typename T_SYSTEM >
        consteval system::type::info CreateInfo( void ) noexcept;

        template< typename T >
        static constexpr auto info_v = system::details::CreateInfo<T>();
    }

    template< typename T_SYSTEM >
    constexpr auto& info_v = details::info_v< std::decay_t<T_SYSTEM> >;

	struct instance
	{
		using query = std::tuple<>;

		PPB_INLINE
		instance( coordinator::instance& Coordinator ) noexcept;
		instance( const instance& ) = delete;

		PPB_INLINE
		void Execute( coordinator::instance& Coordinator ) noexcept;

		coordinator::instance& m_Coordinator;
	};

	namespace details
	{
		template < typename USER_SYSTEM >
		struct completed final : USER_SYSTEM
		{
			PPB_INLINE
			completed( coordinator::instance& Coordinator ) noexcept;

			PPB_FORCEINLINE
			void Run( coordinator::instance& Coordinator ) noexcept;
		};
	}
	
	struct manager
	{
		using SystemMap  = std::unordered_map< system::type::guid, system::instance* >;
		using SystemList = std::vector< std::pair< const system::type::info*, std::unique_ptr<system::instance> > >;

		manager( tools::clock& Clock );
		~manager() = default;

		template < typename... T_SYSTEMS >
		constexpr void RegisterSystems( coordinator::instance& Coordinator ) noexcept;

		template < typename T_SYSTEM >
		constexpr T_SYSTEM& RegisterSystem( coordinator::instance& Coordinator_ ) noexcept;

		template < typename T_SYSTEM >
		T_SYSTEM* FindSystem( void ) noexcept;

		PPB_INLINE
		void Run( coordinator::instance& Coordinator ) noexcept;



		// bool m_bPaused;
		tools::clock&			m_SystemClock;
		SystemMap				m_SystemMap;
		SystemList				m_Systems;
	};
}