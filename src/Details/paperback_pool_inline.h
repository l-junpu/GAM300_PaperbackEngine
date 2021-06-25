	#pragma once

namespace paperback::vm
{
	uint32_t instance::GetPageIndex( const size_t iComponentType, uint32_t Index ) const noexcept
	{
		return (m_ComponentInfo[iComponentType]->m_Size * Index) / paperback::settings::virtual_page_size_v;
	}

	uint32_t instance::GetPageIndex(const component::info& Info, uint32_t Index) const noexcept
	{
		return ((Info.m_Size * Index) - 1) / paperback::settings::virtual_page_size_v;
	}

	void instance::Clear() noexcept
	{
		while (m_CurrentEntityCount)
		{
			Delete(m_CurrentEntityCount-1);
		}
	}

	instance::~instance() noexcept
	{
		Clear();

		for (auto cPool : m_ComponentPool)
		{
			if (cPool)
				VirtualFree(cPool, 0, MEM_RELEASE);
		}
	}

	void instance::Init( int MaxEntites, std::span<const component::info* const> Types ) noexcept
	{
		m_ComponentInfo = Types;
		m_MaxEntityCount = MaxEntites;

		// Reserve memory required for MaxEntites
		for (std::size_t i = 0; i < m_ComponentInfo.size(); i++)
		{
			auto nPages = GetPageIndex( *m_ComponentInfo[i], m_MaxEntityCount ) + 1;
			m_ComponentPool[i] = reinterpret_cast<std::byte*>( VirtualAlloc(nullptr, nPages * paperback::settings::virtual_page_size_v, MEM_RESERVE, PAGE_NOACCESS) );
			assert( m_ComponentPool[i] );
		}
	}


	uint32_t instance::Append() noexcept
	{
		assert( m_CurrentEntityCount < m_MaxEntityCount - 1 );

		// For each valid component
		for (size_t i = 0, end = m_ComponentInfo.size(); i < end; i++)
		{
			auto CInfo	   = *m_ComponentInfo[i];
			auto iCurPage  =  GetPageIndex( CInfo, m_CurrentEntityCount );
			auto iNextpage =  GetPageIndex( CInfo, m_CurrentEntityCount + 1 );

			// Commit new memory in m_ComponentPool's page if the page is full
			if (iCurPage != iNextpage)
			{
				auto pEndOfCurrentPool = m_ComponentPool[i] + iNextpage * paperback::settings::virtual_page_size_v;
				auto pNewPool = VirtualAlloc(pEndOfCurrentPool, paperback::settings::virtual_page_size_v, MEM_COMMIT, PAGE_READWRITE);

				assert(pNewPool == pEndOfCurrentPool);
			}

			// Invoke constructor for Component (If Required)
			if (m_ComponentInfo[i]->m_Constructor)
			{
				m_ComponentInfo[i]->m_Constructor(m_ComponentPool[i] + m_CurrentEntityCount * m_ComponentInfo[i]->m_Size);
			}
		}

		return m_CurrentEntityCount++;
	}

	uint32_t instance::Delete( const uint32_t pool_index ) noexcept
	{
		assert( pool_index < m_CurrentEntityCount && 
				pool_index >= 0 );

		// Return back to the current index
		--m_CurrentEntityCount;

		for (size_t i = 0; i < m_ComponentInfo.size(); ++i)
		{
			const auto& pInfo = *m_ComponentInfo[i];
			auto		pData =  m_ComponentPool[i];

			// Deleting last Entity
			if ( pool_index == m_CurrentEntityCount )
			{
				if ( pInfo.m_Destructor )
					pInfo.m_Destructor(&pData[m_CurrentEntityCount * pInfo.m_Size]);
			}
			// Deleting any other Entity
			else
			{
				if ( pInfo.m_MoveConstructor )
				{
					pInfo.m_MoveConstructor( &pData[pool_index * pInfo.m_Size], &pData[m_CurrentEntityCount * pInfo.m_Size] );
				}
				else
				{
					if ( pInfo.m_Destructor )
						pInfo.m_Destructor(&pData[pool_index * pInfo.m_Size]);
					memcpy( &pData[pool_index * pInfo.m_Size], &pData[m_CurrentEntityCount * pInfo.m_Size], pInfo.m_Size );
				}
			}

			// Free the page if empty
			const auto LastPage = GetPageIndex( pInfo, m_CurrentEntityCount + 1 );
			if ( LastPage != GetPageIndex(pInfo, m_CurrentEntityCount) )
			{
				auto pRaw = &pData[paperback::settings::virtual_page_size_v * LastPage];
				auto b = VirtualFree( pRaw, paperback::settings::virtual_page_size_v, MEM_DECOMMIT );
				assert(b);
			}
		}

		return (m_CurrentEntityCount == 0 || pool_index >= m_CurrentEntityCount)
				? UINT32_MAX
				: GetComponent<component::entity>(pool_index, 0).m_GlobalIndex;
	}


	template < typename T > // OLD
	T& instance::GetComponent( const uint32_t& EntityIndex, const int& ComponentUID ) const noexcept
	{
		// assert is same v T_COMPONENT and std::decay T_COMPONENT
		auto ComponentIndex = GetComponentIndex(ComponentUID);
		return *( reinterpret_cast<T*>( m_ComponentPool[ComponentIndex] + EntityIndex * m_ComponentInfo[ComponentIndex]->m_Size ) );
	}

	int instance::GetComponentIndex(const int& UIDComponent) const noexcept // OLD
	{
		// Find index of component within m_ComponentPool
		for (size_t i = 0, end = m_ComponentInfo.size(); i < end; ++i)
			if (m_ComponentInfo[i]->m_UID == UIDComponent) { return static_cast<int>(i); }

		assert(false);
		return -1;
	}

	template < typename T_COMPONENT > // NEW
	T_COMPONENT& instance::GetComponent( const u32 PoolIndex ) const noexcept
	{
		auto ComponentIndex = GetComponentIndex( component::info_v<T_COMPONENT>.m_UID );

		return *reinterpret_cast< std::decay_t<T_COMPONENT>* >
		(
			&m_ComponentPool[ ComponentIndex ][ PoolIndex * m_ComponentInfo[ComponentIndex]->m_Size] 
		);
	}
}