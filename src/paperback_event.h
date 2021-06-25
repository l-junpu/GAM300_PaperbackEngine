#pragma once

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

		template <auto T_FUNCTION, typename T>
		constexpr void Register( T& Class ) noexcept;

		PPB_INLINE
		void NotifyAll( T_ARGS... Args ) noexcept;

		std::vector<info>		m_Delegates;
	};
}