#pragma once

namespace paperback
{
    namespace archetype
    {
        struct instance
        {
            using PoolDetails   = vm::PoolDetails;
            using EntityIDList  = std::vector<uint16_t>;
            using ComponentPool = std::array<vm::instance, 2>;
            using DeleteList    = std::vector<component::entity>;
            using ComponentInfos = std::span<const component::info* const>;

            coordinator::instance&        m_Coordinator;
            ComponentPool                 m_ComponentPool            {   };           // Component Pool
            ComponentInfos                m_ComponentInfos           {   };           // Component Infos
            DeleteList                    m_DeleteList               {   };           // List of entities to be deleted
            tools::bits                   m_ComponentBits            {   };           // Component Signature
            uint32_t                      m_EntityCount              { 0 };           // Number of Entities within Archetype
            uint32_t                      m_PoolAllocationIndex      { 0 };           // Determines which pool to allocate to - 0 Default
            uint32_t                      m_ProcessReference         { 0 };           // Set to 1 when Archetype is updating

            PPB_INLINE
            instance( coordinator::instance& Coordinator, const tools::bits& ComponentBits ) noexcept;

            PPB_INLINE
            void Init( std::span<const component::info* const> Types ) noexcept;

            template< typename T_CALLBACK >
            PoolDetails CreateEntity( T_CALLBACK&& Function ) noexcept;
            
            PPB_INLINE
            u32 DeleteEntity( const PoolDetails Details ) noexcept;

            PPB_INLINE
            void DestroyEntity( component::entity& Entity ) noexcept;

            template < typename T_COMPONENT >
            T_COMPONENT& GetComponent( const PoolDetails Details ) const noexcept;

            //template < concepts::ReferenceArgs... T_COMPONENTS >
            //constexpr auto GetComponentArray( vm::instance& Pool, u32 PoolIndex, std::tuple<T_COMPONENTS...>* ) const noexcept;

            //template < concepts::MixedArgs... T_COMPONENTS >
            //constexpr auto GetComponentArray( vm::instance& Pool, u32 PoolIndex, std::tuple<T_COMPONENTS...>* ) const noexcept;

            template < typename T_FUNCTION >
            component::entity& TransferExistingEntity( component::entity& Entity, T_FUNCTION&& Function ) noexcept;

            template < typename T_FUNCTION >
            requires( xcore::function::is_callable_v<T_FUNCTION>&&
                      std::is_same_v<typename xcore::function::traits<T_FUNCTION>::return_type, void>&&
                      xcore::function::traits<T_FUNCTION>::arg_count_v == 0 )
            void AccessGuard( T_FUNCTION&& Function ) noexcept;

            PPB_INLINE
            void UpdateStructuralChanges() noexcept;
        };
    }


    namespace entity
    {
        struct info
        {
            archetype::instance*            m_pArchetype = nullptr;
            vm::PoolDetails                 m_PoolDetails;
            component::entity::Validation   m_Validation;
        };

        struct manager
        {
            using PoolDetails       = vm::PoolDetails;
            using EntityListHead    = std::priority_queue<u32>;
            using EntityInfoList    = std::unique_ptr<entity::info[]>;
            using ArchetypeBitsList = std::vector<tools::bits>;
            using ArchetypeList     = std::vector<std::unique_ptr<archetype::instance>>;

            EntityInfoList            m_EntityInfos       = std::make_unique<entity::info[]>( settings::max_entities_v );
            ArchetypeList             m_pArchetypeList    {   };
            ArchetypeBitsList         m_ArchetypeBits     {   };
            uint32_t                  m_EntityIDTracker   { 0 };
            EntityListHead            m_AvailableIndexes  {   };

            PPB_INLINE
            void RegisterEntity( const PoolDetails Details, archetype::instance& Archetype ) noexcept;

            template < typename T_FUNCTION >
            void CreateEntity( T_FUNCTION&& Function, coordinator::instance& Coordinator ) noexcept;

            PPB_INLINE
            void RemoveEntity( const u32 SwappedGlobalIndex, const component::entity DeletedEntity ) noexcept;

            template < typename... T_COMPONENTS >
            archetype::instance& GetOrCreateArchetype( coordinator::instance& Coordinator ) noexcept;

            template < typename... T_COMPONENTS > // PRIVATE FN
            archetype::instance& CreateArchetype( coordinator::instance& Coordinator, const tools::bits& Signature ) noexcept;

            PPB_INLINE
            entity::info& GetEntityInfo( const component::entity Entity ) const noexcept;

            PPB_INLINE
            entity::info& GetEntityInfo( const u32 GlobalIndex ) const noexcept;

            PPB_INLINE
            archetype::instance& GetArchetypeFromEntity(const u32 EntityID) const noexcept;

            PPB_INLINE
            void FreeEntitiesInArchetype( archetype::instance* Archetype ) noexcept;

            template < typename... T_COMPONENTS >
            std::vector<archetype::instance*> Search() const noexcept;

            PPB_INLINE
            archetype::instance* Search( const tools::bits& Bits ) const noexcept;

            PPB_INLINE
            std::vector<archetype::instance*> Search( const tools::query& Query ) const noexcept;



            // PRIVATE
            PPB_INLINE
            u32 AppendEntity() noexcept;

            PPB_INLINE
            archetype::instance& GetOrCreateArchetype( std::span<const component::info* const> Types, coordinator::instance& Coordinator ) noexcept;

            template < typename... T_COMPONENTS >
            std::vector<archetype::instance*> Search( std::span<const component::info* const> Types ) const noexcept;
        };
    }
}