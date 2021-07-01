#pragma once

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