#pragma once

namespace paperback::system
{
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
		struct info
		{
			using call_run = void( system::instance&, coordinator::instance& );

			std::unique_ptr<paperback::system::instance>	m_System;
			call_run*										m_CallRun;
		};

		manager( tools::clock& Clock );
		~manager() = default;


		template < typename... T_SYSTEMS >
		constexpr void RegisterSystems( coordinator::instance& Coordinator ) noexcept;

		template < typename T_SYSTEM >
		constexpr T_SYSTEM& RegisterSystem( coordinator::instance& Coordinator_ ) noexcept;

		// Find System ( Return * )
		
		// Get System  ( Return & )

		PPB_INLINE
		void Run( coordinator::instance& Coordinator ) noexcept;

		tools::clock&		m_SystemClock;
		std::vector<info>	m_Systems;
		// bool m_bPaused;
	};
}