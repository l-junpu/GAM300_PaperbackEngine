#pragma once

namespace paperback::system
{
	//-----------------------------------
	//          System Types
	//-----------------------------------
	namespace type
	{
		using guid = xcore::guid::unit<64, struct system_tag>;

		enum class id : u8
		{
			UPDATE = 0
		};

		enum class call
		{
			CREATED = 0
		,	FRAME_START
		,	PRE_UPDATE
		,	UPDATE
		,	POST_UPDATE
		,	FRAME_END
		};

		struct update
		{
			static constexpr auto id_v = id::UPDATE;

            type::guid                   m_Guid { };
            const char*                  m_pName{ };
		};

		struct info
		{
			using RunSystem = void( coordinator::instance&, system::instance&, type::call );

			const type::guid             m_Guid;
			const type::id               m_TypeID;
										 
			RunSystem*					 m_RunSystem;
			const char*					 m_pName{ };
		};
	}

	//-----------------------------------
	//           System Info
	//-----------------------------------
	namespace details
    {
        template< typename T_SYSTEM >
        consteval system::type::info CreateInfo( void ) noexcept;

        template< typename T >
        static constexpr auto info_v = system::details::CreateInfo<T>();
    }

    template< typename T_SYSTEM >
    constexpr auto& info_v = details::info_v< std::decay_t<T_SYSTEM> >;


	//-----------------------------------
	//        System Interface
	//-----------------------------------
	struct system_interface
	{
		void OnSystemCreated ( void ) noexcept {}	// When RegisterSystem is called - Manual Initialization
        void OnFrameStart    ( void ) noexcept {}
        void PreUpdate       ( void ) noexcept {}
		void Update		     ( void ) noexcept {}
		void PostUpdate      ( void ) noexcept {}
		void OnFrameEnd      ( void ) noexcept {}
	};

	struct instance : system_interface
	{
		// **Note:
		// The System merges "query" and the components listed in "operator()"
		// defined within the class, only iterating Archetypes that fit the
		// generated bitset
		using query = std::tuple<>;

		PPB_INLINE
		instance( coordinator::instance& Coordinator ) noexcept;
		instance( const instance& ) = delete;

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
			void Run( coordinator::instance&, const paperback::system::type::call ) noexcept;
		};
	}
	


	//-----------------------------------
	//          System Manager
	//-----------------------------------
	struct manager
	{
		using SystemMap  = std::unordered_map< system::type::guid, system::instance* >;
		using SystemList = std::vector< std::pair< const system::type::info*, std::unique_ptr<system::instance> > >;

		manager( tools::clock& Clock );
		manager( const manager& ) = delete;
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