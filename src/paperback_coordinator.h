#pragma once

namespace paperback::coordinator
{
	struct instance final
	{
	//private:
		
		using ArchetypeList = std::vector<std::unique_ptr<archetype::instance>>;
		using ArchetypeBitsList = std::vector<tools::bits>;

		//std::unique_ptr< game_mgr::instance	  >			m_GameMgr    = std::make_unique< game_mgr::instance	>( *this );
		std::unique_ptr< component::manager	  >			m_CompMgr    = std::make_unique< component::manager	>(       );
		std::unique_ptr< entity::manager >				m_EntityMgr  = std::make_unique< entity::manager    >(       );
		std::unique_ptr< system::manager >				m_SystemMgr  = std::make_unique< system::manager    >(       );

		ArchetypeList                  m_pArchetypeList;                                     // List of Archetypes
		ArchetypeBitsList              m_ArchetypeBits;                                      // Bit Signature of Archetypes

		bool											m_GameActive = true;

	//public:

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
		void CreateEntity( T_FUNCTION&& ) noexcept;

		PPB_INLINE
		void DeleteEntity( component::Entity& ) noexcept;

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
		const entity::info& GetEntityInfo( component::Entity& ) const noexcept;

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