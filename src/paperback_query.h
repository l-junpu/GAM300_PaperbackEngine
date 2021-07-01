#pragma once

namespace paperback::query
{
    template < typename... T_COMPONENTS >
    struct must
    {
        using type = std::tuple< T_COMPONENTS... >;;
    };

    template < typename... T_COMPONENTS >
    struct one_of
    {
        using type = std::tuple< T_COMPONENTS... >;;
    };

    template < typename... T_COMPONENTS >
    struct none_of
    {
        using type = std::tuple< T_COMPONENTS... >;;
    };
}

namespace tools
{
    struct query
    {
        bits    m_Must;
        bits    m_OneOf;
        bits    m_NoneOf;

        PPB_INLINE
        bool Compare( const bits& ArchetypeBits ) const noexcept;

        template< typename T_FUNCTION >
        void AddQueryFromFunction( T_FUNCTION&& ) noexcept;

        template<typename... T_QUERIES >
        void AddQueryFromTuple( std::tuple<T_QUERIES...>* ) noexcept;
    };
}