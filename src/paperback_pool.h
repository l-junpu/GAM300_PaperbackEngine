#pragma once

namespace paperback::vm
{
	struct PoolDetails // Change to u16 afterwards
	{
		u32 m_Key			= settings::invalid_index_v;	// Access Pool Within Archetype
		u32 m_PoolIndex		= settings::invalid_index_v;	// Access Component Within Pool
	};

	struct instance
	{
	//private:

		std::span<const component::info* const>										m_ComponentInfo				{   };
		std::array<std::byte*, paperback::settings::max_components_per_entity_v>	m_ComponentPool				{   };
		uint32_t																	m_CurrentEntityCount		{ 0 };

		PPB_INLINE
		u32 GetPageIndex( const size_t LocalComponentIndex, const u32 Count ) const noexcept;

		PPB_INLINE
		u32 GetPageIndex( const component::info& Info, const u32 Count ) const noexcept;

		PPB_INLINE
		void Clear() noexcept;

	//public:

		instance( void )			noexcept = default;
		instance( const instance& ) noexcept = delete;

		~instance() noexcept;

		PPB_INLINE
		void Init( std::span<const component::info* const> Types ) noexcept;

		PPB_INLINE
		u32 Append() noexcept;

		PPB_INLINE
		u32 Delete( const u32 PoolIndex ) noexcept;

		template < typename T_COMPONENT >
		T_COMPONENT& GetComponent( const u32 PoolIndex ) const noexcept;

		PPB_INLINE
		int GetComponentIndex( const u32 UIDComponent ) const noexcept;
	};
}