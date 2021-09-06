#pragma once

namespace paperback::coordinator
{
	struct instance final
	{
		tools::clock				m_Clock;
		component::manager			m_CompMgr;
		entity::manager				m_EntityMgr;
		system::manager				m_SystemMgr{ m_Clock };
		bool						m_GameActive = true;

	//public:

		instance( void ) noexcept;
		instance( const instance& ) = delete;

		PPB_INLINE
		static instance& GetInstance() noexcept;

		PPB_INLINE
		void Update() noexcept;



		//-----------------------------------
		//           Registration
		//-----------------------------------

		template < concepts::System... T_SYSTEMS >
		constexpr void RegisterSystems() noexcept;
		
		template< typename... T_COMPONENTS >
		constexpr void RegisterComponents( void ) noexcept;

		

		//-----------------------------------
		//    Archetype / Entity Methods
		//-----------------------------------

		template < typename... T_COMPONENTS >
		archetype::instance& GetOrCreateArchetype() noexcept;

		template< typename T_FUNCTION >
		void CreateEntity( T_FUNCTION&& Function ) noexcept;

		template< typename T_FUNCTION >
		void CreateEntities( T_FUNCTION&& Function
						   , const u32 Count ) noexcept;

		PPB_INLINE
		void DeleteEntity( component::entity& Entity ) noexcept;

		PPB_INLINE
		void RemoveEntity( const uint32_t SwappedGlobalIndex, const component::entity Entity ) noexcept;

		/*
		template < concepts::TupleSpecialization T_TUPLE_ADD
				 , concepts::TupleSpecialization T_TUPLE_REMOVE = std::tuple<>
				 , concepts::Callable			 T_FUNCTION     = empty_lambda >
		component::entity AddOrRemoveComponents( component::entity Entity
											   , T_FUNCTION&& Function ) noexcept;
		*/


		//-----------------------------------
		//           Query Methods
		//-----------------------------------

		template < typename... T_COMPONENTS >
        std::vector<archetype::instance*> Search() const noexcept;

		PPB_INLINE
        archetype::instance* Search( const tools::bits& Bits ) const noexcept;

        PPB_INLINE
        std::vector<archetype::instance*> Search( const tools::query& Query ) const noexcept;
        


		//-----------------------------------
		//           Game Loops
		//-----------------------------------

        template < concepts::Callable_Void T_FUNCTION>
        void ForEach( const std::vector<archetype::instance*>& ArchetypeList
					, T_FUNCTION&& Function ) noexcept;

        template < concepts::Callable_Bool T_FUNCTION>
        void ForEach( const std::vector<archetype::instance*>& ArchetypeList
					, T_FUNCTION&& Function ) noexcept;



		//-----------------------------------
		//        Getters / Setters
		//-----------------------------------

		PPB_INLINE
		entity::info& GetEntityInfo( component::entity& Entity ) const noexcept;

		PPB_INLINE
        entity::info& GetEntityInfo( const u32 GlobalIndex ) const noexcept;

		template< typename T_SYSTEM >
		T_SYSTEM* FindSystem( void ) noexcept;

		template< typename T_SYSTEM >
		T_SYSTEM& GetSystem( void ) noexcept;

		PPB_INLINE // To Place Inside Archetype Directly
		void FreeEntitiesInArchetype( archetype::instance* Archetype ) noexcept;

		PPB_FORCEINLINE
		float DeltaTime() const noexcept;

		PPB_FORCEINLINE
        void SetTimeScale( const float s = 1.0f ) noexcept;

		PPB_FORCEINLINE
        float GetTimeScale() const noexcept;

		

		//-----------------------------------
		//    Archetype / Entity Methods
		//-----------------------------------
		
		/*
		template < concepts::Callable T_FUNCTION >
		component::entity AddOrRemoveComponents( const component::entity Entity
											   , std::span<const component::info* const> Add
											   , std::span<const component::info* const> Remove
											   , T_FUNCTION&& Function = empty_lambda{} ) noexcept;
		*/
	};
}

static struct engine
{
	paperback::coordinator::instance& m_Coordinator = paperback::coordinator::instance::GetInstance();
	int m_Width = 1024;
	int m_Height = 800;
} m_Engine;

#define PPB m_Engine.m_Coordinator