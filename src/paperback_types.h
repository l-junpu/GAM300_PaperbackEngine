#pragma once

namespace paperback
{
	using empty_lambda = decltype([](){});

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
}