#pragma once

namespace paperback::coordinator
{
	struct instance final
	{
	//private:
		
		using ArchetypeList = std::vector<std::unique_ptr<archetype::instance>>;
		using ArchetypeBitsList = std::vector<tools::bits>;

		component::manager			m_CompMgr;
		entity::manager				m_EntityMgr;
		system::manager				m_SystemMgr;

		ArchetypeList               m_pArchetypeList;                // List of Archetypes
		ArchetypeBitsList           m_ArchetypeBits;                 // Bit Signature of Archetypes

		bool						m_GameActive = true;

	//public:

		instance( void ) noexcept;

		PPB_INLINE
		static instance& GetInstance() noexcept;

		PPB_INLINE
		void Update() noexcept;

		template < typename... T_SYSTEMS >
		constexpr void RegisterSystems() noexcept;
		
		template< typename... T_COMPONENTS >
		constexpr void RegisterComponents( void ) noexcept;

		template < typename... T_COMPONENTS >
		archetype::instance& GetOrCreateArchetype() noexcept;

		template< typename T_FUNCTION >
		void CreateEntity( T_FUNCTION&& Function ) noexcept;

		template< typename T_FUNCTION >
		void CreateEntities( T_FUNCTION&& Function, const u32 Count ) noexcept;

		PPB_INLINE
		void DeleteEntity( component::entity& Entity ) noexcept;

		PPB_INLINE // TEMPORARY TEST ONLY
		void RemoveEntity(const uint32_t LastIndex, const vm::PoolDetails Details) noexcept;

		template < typename... T_COMPONENTS >
        std::vector<archetype::instance*> Search() const noexcept;

        PPB_INLINE
        std::vector<archetype::instance*> Search( const tools::query& Query ) const noexcept;
        
        template < typename T_FUNCTION>
        requires( xcore::function::is_callable_v<T_FUNCTION> &&
                  std::is_same_v< void, typename xcore::function::traits<T_FUNCTION>::return_type > )
        void ForEach( const std::vector<archetype::instance*>& ArchetypeList, T_FUNCTION&& Function ) noexcept;

        template < typename T_FUNCTION>
        requires( xcore::function::is_callable_v<T_FUNCTION> && 
			      std::is_same_v< bool, typename xcore::function::traits<T_FUNCTION>::return_type > )
        void ForEach( const std::vector<archetype::instance*>& ArchetypeList, T_FUNCTION&& Function ) noexcept;

		PPB_INLINE
		entity::info& GetEntityInfo( component::entity& Entity ) const noexcept;

		PPB_INLINE
        entity::info& GetEntityInfo( const u32 GlobalIndex ) const noexcept;

		PPB_INLINE // TO REPLACE IN ARCHETYPE
		void FreeEntitiesInArchetype( archetype::instance* ) noexcept;


	// private:
		PPB_INLINE // NEW
        archetype::instance& GetOrCreateArchetype( std::span<const component::info* const> Types ) noexcept;

		template < typename... T_COMPONENTS >
        std::vector<archetype::instance*> Search( std::span<const component::info* const> Types ) const noexcept;
	};
}

static struct engine
{
	paperback::coordinator::instance& m_Coordinator = paperback::coordinator::instance::GetInstance();
	int m_Width = 1024;
	int m_Height = 800;
}m_Engine;