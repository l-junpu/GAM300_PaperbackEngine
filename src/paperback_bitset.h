#pragma once

namespace tools
{
    struct bits
    {
        std::array<uint64_t, 4> m_bits{};

        PPB_FORCEINLINE
        void Set( int Bit ) noexcept;

        PPB_FORCEINLINE
        bool Has( int Bit ) const noexcept;
		
        PPB_INLINE
        bool Match(const bits& B) const noexcept;

        PPB_INLINE
        bool Compare(const bits& B) const noexcept;

        template < typename... T_COMPONENTS >
        void AddFromComponents();
    };
}