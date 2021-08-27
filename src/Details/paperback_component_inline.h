#pragma once

namespace paperback::component
{
    // Info
    namespace details
    {
        template< typename T_COMPONENT >
        consteval info CreateInfo( void ) noexcept
        {
            return info
            {
                .m_Guid        = std::is_same_v< component::entity, T_COMPONENT >
                                 ? type::guid{ nullptr }
                                 : T_COMPONENT::typedef_v.m_Guid.m_Value
                                     ? T_COMPONENT::typedef_v.m_Guid
                                     : type::guid{ __FUNCSIG__ }
            ,   .m_TypeID      = T_COMPONENT::typedef_v.id_v
            ,   .m_UID         = info::invalid_id_v
            ,   .m_Size        = static_cast<uint32_t>( sizeof(T_COMPONENT) )
            ,   .m_Constructor = std::is_trivially_constructible_v<T_COMPONENT>
                                 ? nullptr
                                 : []( std::byte* p ) noexcept
                                   {
                                       new(p) T_COMPONENT;
                                   }
            ,   .m_Destructor  = std::is_trivially_destructible_v<T_COMPONENT>
                                 ? nullptr
                                 : []( std::byte* p ) noexcept
                                   {
                                       std::destroy_at( reinterpret_cast<T_COMPONENT*>(p) );
                                   }
            ,   .m_Move        = std::is_trivially_move_assignable_v<T_COMPONENT>
                                 ? nullptr
                                 : [](std::byte* d, std::byte* s) noexcept
                                   {
                                       *reinterpret_cast<T_COMPONENT*>(d) = std::move( *reinterpret_cast<T_COMPONENT*>(s) );
                                   }
            };
        }


        template< typename T_COMPONENT_A, typename T_COMPONENT_B >
        struct component_comparator
        {
            constexpr static bool value = component::info_v< T_COMPONENT_A >.m_Guid
                                        < component::info_v< T_COMPONENT_B >.m_Guid;
        };

        template< paperback::concepts::TupleSpecialization T_TUPLE >
        using sort_tuple_t = xcore::types::tuple_sort_t< details::component_comparator, T_TUPLE >;


        // Find Component Index in Container
        template < typename T_CONTAINER >
        static constexpr auto find_component_index_v = [&]( T_CONTAINER& Container, component::info* Info, size_t Count )
        {
            return static_cast<size_t>( std::upper_bound( std::begin( Container )
                                                        , std::begin( Container ) + Count
                                                        , Info
                                                        , []( const component::info* pA, const component::info* pB)
                                                          {
                                                              return pA->m_UID < pB->m_UID;
                                                          }
                                                        ) - std::begin( Container )
                                      );
        };
    }

    template< paperback::concepts::TupleSpecialization T_TUPLE >
    static constexpr auto sorted_info_array_v = []<typename...T>( std::tuple<T...>* ) constexpr
    {
        if constexpr ( sizeof...(T) == 0 ) return std::span<const component::info*>{};
        else                               return std::array{ &component::info_v<T>... };
    }( xcore::types::null_tuple_v< sort_tuple_t<T_TUPLE> > );

	
    // Manager
    template< paperback::component::concepts::ValidComponent T_COMPONENT >
    void manager::RegisterComponent( void ) noexcept
    {
        if (component::info_v<T_COMPONENT>.m_UID == component::info::invalid_id_v)
            component::info_v<T_COMPONENT>.m_UID = m_ComponentUID++;
    }

    template< typename... T_COMPONENTS >
    void manager::RegisterComponents( void ) noexcept
    {
        ( (RegisterComponent<T_COMPONENTS>()), ... );
    }

    constexpr bool entity::Validation::operator == ( const Validation& V ) const noexcept
    {
        return m_UID == V.m_UID;
    }

    constexpr bool entity::IsZombie( void ) const noexcept
    {
        return m_Validation.m_bZombie;
    }

    constexpr bool entity::operator == ( const entity& Entity ) const noexcept
    {
        return m_GlobalIndex == Entity.m_GlobalIndex;
    }
}