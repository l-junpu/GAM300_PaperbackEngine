#pragma once

namespace paperback
{
    namespace archetype
    {
        struct instance
        {
            using PoolDetails = vm::PoolDetails;
            using EntityIDList = std::vector<uint16_t>;
            using ComponentPool = std::array<vm::instance, 2>;
            using DeleteList = std::vector<component::entity>;

            ComponentPool                 m_ComponentPool{   };                 // Component Pool
            DeleteList                    m_DeleteList{   };                    // List of entities to be deleted
            tools::bits                   m_ComponentBits{   };                 // Component Signature
            uint32_t                      m_EntityCount{ 0 };                   // Number of Entities within Archetype
            uint32_t                      m_PoolAllocationIndex{ 0 };           // Determines which pool to allocate to - 0 Default
            uint32_t                      m_ProcessReference{ 0 };              // Set to 1 when Archetype is updating

            PPB_INLINE
            instance(const tools::bits& ComponentBits) noexcept;

            PPB_INLINE
            void Init(std::span<const component::info* const> Types) noexcept;

            //template< typename T_CALLBACK >
            //uint32_t CreateEntity(T_CALLBACK&& Function) noexcept;

            //NEW TEST
            template< typename T_CALLBACK >
            PoolDetails CreateEntity( T_CALLBACK&& Function ) noexcept; // basically normal entity creation, returning pool index and pool allocation index

            //PPB_INLINE // OLD
            //uint32_t DeleteEntity(const uint32_t EntityID) noexcept;

            PPB_INLINE // NEW
            uint32_t DeleteEntity( const PoolDetails Details ) noexcept;

            PPB_INLINE
            void DestroyEntity(component::entity& Entity) noexcept;

            template < typename T > //OLD
            T& GetComponent(const uint32_t& EntityIndex, const int& ComponentUID) const noexcept;

            template < typename T > //OLD
            T& GetComponent(const uint32_t& EntityIndex) const noexcept;

            template < typename T_COMPONENT >
            T_COMPONENT& GetComponent( const PoolDetails Details ) const noexcept;

            template < typename T_FUNCTION >
            requires( xcore::function::is_callable_v<T_FUNCTION>&&
                      std::is_same_v<typename xcore::function::traits<T_FUNCTION>::return_type, void>&&
                      xcore::function::traits<T_FUNCTION>::arg_count_v == 0 )
            void AccessGuard( T_FUNCTION&& Function ) noexcept;
        };
    }


    namespace entity
    {
        struct info
        {
            archetype::instance*      m_pArchetype = nullptr;
            //int                       m_PoolIndex = -1;                        // Index within the Archetype (Local)
            vm::PoolDetails           m_PoolDetails;
            uint32_t                  m_Generation = 0;                        // Entity generation
            //uint8_t                   m_PoolCount = 0;                         // Pool 0 / 1
        };

        struct manager
        {
            using PoolDetails    = vm::PoolDetails;
            using EntityInfoList = std::unique_ptr<entity::info[]>;

            EntityInfoList                 m_EntityInfos = std::make_unique<entity::info[]>(100000);           // Contains information of each entity
            uint32_t                       m_EntityIDTracker = 0;                                              // Global EntityID tracker (Assigned to EntityID Component)

            //PPB_INLINE // OLD
            //void RegisterEntity( const u32 entity_id, archetype::instance* archetype ) noexcept;

            PPB_INLINE // NEW
            void RegisterEntity( const PoolDetails Details, archetype::instance* archetype ) noexcept;

            PPB_INLINE // OLD
            void RemoveEntity( const u32 LastIndex, const uint32_t DeletedIndex ) noexcept;

            PPB_INLINE // NEW
            void RemoveEntity( const u32 LastIndex, const PoolDetails Details ) noexcept;

            PPB_INLINE
            entity::info& GetEntityInfo( const component::entity Entity ) const noexcept;

            PPB_INLINE
            entity::info& GetEntityInfo( const u32 GlobalIndex ) const noexcept;

            PPB_INLINE
            archetype::instance& GetArchetypeFromEntity(const u32 EntityID) const noexcept;

            PPB_INLINE
            void FreeEntitiesInArchetype( archetype::instance* Archetype ) noexcept;
        };
    }
}