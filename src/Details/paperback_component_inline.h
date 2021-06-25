#pragma once

namespace paperback::component
{
    namespace details
    {
        template< typename T_COMPONENT >
        consteval info CreateInfo( void ) noexcept
        {
            return info
            {
                .m_UID = info::invalid_id_v
            ,   .m_Size = static_cast<uint32_t>( sizeof(T_COMPONENT) )
            ,   .m_Constructor = std::is_trivially_constructible_v<T_COMPONENT>
                    ? nullptr
                    : []( std::byte* p ) noexcept
                      {
                          new(p) T_COMPONENT;
                      }
            ,   .m_Destructor = std::is_trivially_destructible_v<T_COMPONENT>
                    ? nullptr
                    : []( std::byte* p ) noexcept
                      {
                          std::destroy_at( reinterpret_cast<T_COMPONENT*>(p) );
                      }
            ,   .m_MoveConstructor = std::is_trivially_move_assignable_v<T_COMPONENT>
                    ? nullptr
                    : [](std::byte* d, std::byte* s) noexcept
                      {
                          *reinterpret_cast<T_COMPONENT*>(d) = std::move( *reinterpret_cast<T_COMPONENT*>(s) );
                      }
            };
        }
    }

	
    template< typename T_COMPONENT >
    void manager::RegisterComponent( void ) noexcept
    {
        if (component::info_v<T_COMPONENT>.m_UID == component::info::invalid_id_v)
            component::info_v<T_COMPONENT>.m_UID = m_ComponentUID++;
    }

    // Register Multiple Components
    template< typename... T_COMPONENTS >
    void manager::RegisterComponents( void ) noexcept
    {
        ( (RegisterComponent<T_COMPONENTS>()), ... );
    }

    constexpr bool entity::Validation::operator == ( const Validation& v ) const noexcept
    {
        return m_UID == v.m_UID;
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