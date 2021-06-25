//#pragma once
//
//namespace paperback::game_mgr
//{
//    struct instance
//    {
//    //private:
//
//        using ArchetypeList        =   std::vector<std::unique_ptr<archetype::instance>>;
//        using ArchetypeBitsList    =   std::vector<tools::bits>;
//
//        coordinator::instance&         m_Coordinator;                                        // Coordinator Instance
//        ArchetypeList                  m_pArchetypeList;                                     // List of Archetypes
//        ArchetypeBitsList              m_ArchetypeBits;                                      // Bit Signature of Archetypes
//
//    //public:
//
//        instance( coordinator::instance& ) noexcept;
//        ~instance() noexcept = default;
//
//        template < typename... T_COMPONENTS >
//        archetype::instance& GetOrCreateArchetype() noexcept;
//
//        template < typename... T_COMPONENTS >
//        std::vector<archetype::instance*> Search() const noexcept;
//
//        PPB_INLINE
//        std::vector<archetype::instance*> Search( const tools::query& Query ) const noexcept;
//        
//        template < typename T_FUNCTION>
//        requires( xcore::function::is_callable_v<T_FUNCTION> &&
//                  std::is_same_v< void, typename xcore::function::traits<T_FUNCTION>::return_type > )
//        void ForEach( const std::vector<archetype::instance*>& ArchetypeList, T_FUNCTION&& Function ) noexcept;
//
//        template < typename T_FUNCTION>
//        requires( xcore::function::is_callable_v<T_FUNCTION> && 
//			      std::is_same_v< bool, typename xcore::function::traits<T_FUNCTION>::return_type > )
//        void ForEach( const std::vector<archetype::instance*>& ArchetypeList, T_FUNCTION&& Function ) noexcept;
//
//    //private:
//
//        PPB_INLINE
//        archetype::instance& GetOrCreateArchetype(std::span<const component::info* const> Types) noexcept;
//
//        template < typename... T_COMPONENTS >
//        std::vector<archetype::instance*> Search(std::span<const component::info* const> Types) const noexcept;
//    };
//}