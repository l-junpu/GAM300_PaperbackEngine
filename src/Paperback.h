#ifndef PAPERBACK_H
#define PAPERBACK_H
#pragma once

//----------------------------------
// Defines
//----------------------------------
#ifdef PAPERBACK_DEBUG
#define PPB_INLINE
#define PPB_FORCEINLINE
#else
#define PPB_INLINE inline
#define PPB_FORCEINLINE __inline
#endif

//----------------------------------
// STL
//----------------------------------
#define NOMINMAX
#include "Windows.h"
#include <iostream>
#include <vector>
#include <array>
#include <queue>
#include <functional>
#include <typeindex>

//----------------------------------
// Dependencies
//----------------------------------
#include "../dependencies/xcore/src/xcore.h"
#define GLUT_STATIC_LIB
#include "../dependencies/glut/include/GL/glut.h"

//----------------------------------
// Forward Declaration
//----------------------------------
namespace paperback
{
	namespace coordinator
	{
		struct instance;
	}
	namespace system
	{
		struct instance;
	}
	namespace component
	{
		struct info;
	}
}

//----------------------------------
// Files
//----------------------------------
#include "paperback_logger.h"
#include "paperback_types.h"
#include "paperback_settings.h"
#include "paperback_event.h"
#include "paperback_clock.h"
#include "paperback_event.h"
#include "paperback_component.h"
#include "paperback_bitset.h"
#include "paperback_query.h"
#include "paperback_pool.h"
#include "paperback_entity.h"
#include "paperback_system.h"
#include "paperback_coordinator.h"

//----------------------------------
// Inline Files
//----------------------------------
#include "Details/paperback_logger_inline.h"
#include "Details/paperback_event_inline.h"
#include "Details/paperback_clock_inline.h"
#include "Details/paperback_component_inline.h"
#include "Details/paperback_system_inline.h"
#include "Details/paperback_pool_inline.h"
#include "Details/paperback_entity_inline.h"
#include "Details/paperback_query_inline.h"
#include "Details/paperback_bitset_inline.h"
#include "Details/paperback_event_inline.h"
#include "Details/paperback_coordinator_inline.h"

#endif