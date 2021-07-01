#pragma once

namespace tools
{
	bool query::Compare( const bits& ArchetypeBits ) const noexcept
	{
		uint64_t oneOfCount{}, atLeastOne{};
	
		for (int i = 0, size = static_cast<int>(ArchetypeBits.m_bits.size()); i < size; ++i)
		{
			if ((m_Must.m_bits[i] & ArchetypeBits.m_bits[i]) != m_Must.m_bits[i])
				return false;
	
			oneOfCount |= (m_OneOf.m_bits[i] & ArchetypeBits.m_bits[i]);
			atLeastOne |= m_OneOf.m_bits[i];
	
			if ((m_NoneOf.m_bits[i] & ArchetypeBits.m_bits[i]))
				return false;
		}
	
		return atLeastOne ? oneOfCount : true;
	}
	
	template< typename T_FUNCTION >
	void query::AddQueryFromFunction( T_FUNCTION&& ) noexcept
	{
		using func_traits = xcore::function::traits<T_FUNCTION>;
	
		[&]<typename...T_COMPONENTS>( std::tuple<T_COMPONENTS...>* )
		{
			auto Component_Query = [&]<typename T_C>( std::tuple<T_C>* )
			{
				if constexpr (std::is_pointer_v<T_C>)
				{
					m_OneOf.AddFromComponents<T_C>();
				}
				else if constexpr (std::is_reference_v<T_C>)
				{
					m_Must.AddFromComponents<T_C>();
				}
				else
				{
					static_assert( xcore::types::always_false_v<T_C> );
				}
			};
	
			( Component_Query(reinterpret_cast<std::tuple<T_COMPONENTS>*>( nullptr )), ... );
	
		}( reinterpret_cast<func_traits::args_tuple*>( nullptr ) );
	}
	
	template<typename... T_QUERIES >
	void query::AddQueryFromTuple( std::tuple<T_QUERIES...>* ) noexcept
	{
		auto Param_Query = [&]<template<typename...> class T_QTYPE, typename... T_COMPONENTS>( T_QTYPE<T_COMPONENTS...>* )
		{
			using Query_Tuple = T_QTYPE<T_COMPONENTS...>;
	
			if constexpr ( std::is_same_v< Query_Tuple, paperback::query::must<T_COMPONENTS...> > )
			{
				m_Must.AddFromComponents<T_COMPONENTS...>();
			}
			else if constexpr ( std::is_same_v< Query_Tuple, paperback::query::one_of<T_COMPONENTS...> > )
			{
				m_OneOf.AddFromComponents<T_COMPONENTS...>();
			}
			else if constexpr ( std::is_same_v< Query_Tuple, paperback::query::none_of<T_COMPONENTS...> > )
			{
				m_NoneOf.AddFromComponents<T_COMPONENTS...>();
			}
			else
			{
				static_assert( xcore::types::always_false_v<Query_Tuple> );
			}
		};
	
		( Param_Query(reinterpret_cast<T_QUERIES*>(nullptr)), ... );
	}
}