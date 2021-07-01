#pragma once

namespace tools
{
    struct bits
    {
        std::array<uint64_t, 4> m_bits{};

        PPB_FORCEINLINE
        void Set( const int Bit ) noexcept;

        PPB_FORCEINLINE
        bool Has( const int Bit ) const noexcept;
		
        PPB_INLINE
        bool Match( const bits& Query ) const noexcept;

        PPB_INLINE
        bool Compare( const bits& Query ) const noexcept;

        template < typename... T_COMPONENTS >
        void AddFromComponents();
    };
}