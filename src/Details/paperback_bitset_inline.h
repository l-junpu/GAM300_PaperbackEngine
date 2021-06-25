#pragma once

namespace tools
{
    void bits::Set( int Bit ) noexcept
    {
		int x = Bit / 64;
		int y = Bit % 64;
		m_bits[x] |= (1ull << y);
    }

    bool bits::Has( int Bit ) const noexcept
    {
		int x = Bit / 64;
		int y = Bit % 64;
		
		return !!(m_bits[x] & (1ull << y));
    }

    bool bits::Match(const bits& B) const noexcept
    {
		for (size_t i = 0, size = m_bits.size(); i < size; ++i)
        {
            if ((m_bits[i] & B.m_bits[i]) != m_bits[i])
                return false;
        }

        return true;
    }

    bool bits::Compare(const bits& B) const noexcept
    {
		for (size_t i = 0, size = m_bits.size(); i < size; ++i)
        {
            if ((m_bits[i] & B.m_bits[i]) != B.m_bits[i])
                return false;
        }

        return true;
    }

    template < typename... T_COMPONENTS >
    void bits::AddFromComponents()
    {
        ( (Set( paperback::component::info_v<T_COMPONENTS>.m_UID)), ... );
    }
}