#pragma once

namespace paperback
{
	//----------------------------------
	// Typedefs
	//----------------------------------
	using u8  = std::uint8_t;
	using u16 = std::uint16_t;
	using u32 = std::uint32_t;
	using u64 = std::uint64_t;

	constexpr u8  u8_max  = UINT8_MAX;
	constexpr u16 u16_max = UINT16_MAX;
	constexpr u32 u32_max = UINT32_MAX;
	constexpr u64 u64_max = UINT64_MAX;

	using i8  = std::int8_t;
	using i16 = std::int16_t;
	using i32 = std::int32_t;
	using i64 = std::int64_t;

	constexpr i8  i8_min  = INT8_MIN;
	constexpr i8  i8_max  = INT8_MAX;
	constexpr i16 i16_min = INT16_MIN;
	constexpr i16 i16_max = INT16_MAX;
	constexpr i32 i32_min = INT32_MIN;
	constexpr i32 i32_max = INT32_MAX;
	constexpr i64 i64_min = INT64_MIN;
	constexpr i64 i64_max = INT64_MAX;

	using f32 = float;
	using f64 = double;


	//----------------------------------
	// Function Typedefs
	//----------------------------------
	using empty_lambda						= decltype([]() {});

	template < typename T_FUNCTION >
	using FN_RETURN_TYPE					= typename xcore::function::traits<T_FUNCTION>::return_type;


	//----------------------------------
	// Details
	//----------------------------------
	namespace details
	{

	}


	//----------------------------------
	// Concepts C++20
	//----------------------------------
	namespace concepts
	{
		template < typename T_SYSTEM >
		concept System						= std::derived_from< T_SYSTEM, system::instance >;

		template< typename T_FUNCTION = empty_lambda >
		concept Callable					= xcore::function::is_callable_v< T_FUNCTION >;

		template< typename T_FUNCTION >
		concept Callable_Void				= Callable< T_FUNCTION >
										   && std::is_same_v< void, FN_RETURN_TYPE< T_FUNCTION > >;

		template< typename T_FUNCTION >
		concept Callable_Bool				= Callable< T_FUNCTION >
										   && std::is_same_v< bool, FN_RETURN_TYPE< T_FUNCTION > >;

		template < typename T_CONTAINER >
		concept TupleSpecialization			= xcore::types::is_specialized_v< std::tuple, T_CONTAINER >;

		template < typename... T_ARGS >
		concept ReferenceArgs				= ( std::is_reference_v<T_ARGS> && ... );

		template < typename... T_ARGS >
		concept MixedArgs					= !( std::is_reference_v<T_ARGS> && ... );


		//template < typename T_CONTAINER >
		//concept STL_Iteratable = requires( T_CONTAINER Container )
		//{
		//	{ Container.begin() } -> 
		//};
	}
}