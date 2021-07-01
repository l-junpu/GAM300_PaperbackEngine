#pragma once

namespace paperback::component
{
    struct info
    {
        constexpr static auto invalid_id_v = 0xffff;

        using Constructor   =  void( std::byte* ) noexcept;
        using Destructor    =  void( std::byte* ) noexcept;
        using Move          =  void( std::byte* Destination, std::byte* Source ) noexcept;

        mutable int            m_UID;
        mutable uint32_t       m_Size;
        Constructor*           m_Constructor;
        Destructor*            m_Destructor;
        Move*                  m_Move;
    };


    namespace details
    {
        template< typename T_COMPONENT >
        consteval info CreateInfo( void ) noexcept;

        template< typename T >
        static constexpr auto info_v = component::details::CreateInfo<T>();
    }

    // component::info_v<T_COMPONENT>
    template< typename T_COMPONENT >
    constexpr auto& info_v = details::info_v< std::decay_t<T_COMPONENT> >;
	

    union entity final
    {
        union Validation final
        {
            uint32_t        m_UID;
            struct
            {
                uint32_t    m_Next : 31         // Index of next Entity within Entity Manager (For Deletion) - Currently Unused
                ,           m_bZombie    : 1;   // Entity Status
            };

            PPB_FORCEINLINE
            constexpr bool operator == ( const Validation& V ) const noexcept;
        };
        static_assert( sizeof(Validation) == sizeof(uint32_t) );

        uint64_t            m_UID;
        struct
        {
            uint32_t        m_GlobalIndex;      // Index of Entity within Entity Manager
            Validation      m_Validation;       // Entity Status
        };

        PPB_FORCEINLINE
        constexpr bool IsZombie( void ) const noexcept;

        PPB_FORCEINLINE
        constexpr bool operator == ( const entity& Entity ) const noexcept;
    };
    static_assert( sizeof(entity) == sizeof(uint64_t) );
    

    // paperback::component::mgr
	struct manager
    {
    private:
        inline static int m_ComponentUID = 0;

    public:

        template< typename T_COMPONENT >
        void RegisterComponent( void ) noexcept;

        template< typename... T_COMPONENTS >
        void RegisterComponents( void ) noexcept;
    };
}