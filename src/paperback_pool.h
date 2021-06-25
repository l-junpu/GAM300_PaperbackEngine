#pragma once

namespace paperback::vm
{
	struct instance
	{
	//private:

		std::span<const component::info* const>										m_ComponentInfo				{   };				// Component Infos
		std::array<std::byte*, paperback::settings::max_components_per_entity_v>	m_ComponentPool				{   };				// Array of Component Pools
		uint32_t																	m_MaxEntityCount			{ 0 };				// Max Entity Count
		uint32_t																	m_CurrentEntityCount		{ 0 };				// Entity Count

		PPB_INLINE
		uint32_t GetPageIndex( const size_t iComponentType, uint32_t Index ) const noexcept;

		PPB_INLINE
		uint32_t GetPageIndex( const component::info& Info, uint32_t Index ) const noexcept;

		PPB_INLINE
		void Clear() noexcept;

	//public:

		instance( void )			noexcept = default;
		instance( const instance& ) noexcept = delete;

		~instance() noexcept;

		PPB_INLINE
		void Init( int MaxEntites, std::span<const component::info* const> Types ) noexcept;

		PPB_INLINE
		uint32_t Append() noexcept;

		PPB_INLINE
		uint32_t Delete( const uint32_t pool_index ) noexcept;

		template < typename T >
		T& GetComponent( const uint32_t& EntityIndex, const int& ComponentUID ) const noexcept;

		PPB_INLINE
		int GetComponentIndex(const int& UIDComponent) const noexcept;
	};
}