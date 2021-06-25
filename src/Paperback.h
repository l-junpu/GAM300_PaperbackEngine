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
#include <functional>

//----------------------------------
// Dependencies
//----------------------------------
#include "../dependencies/xcore/src/xcore.h"
#define GLUT_STATIC_LIB
#include "../dependencies/glut/include/GL/glut.h"

//----------------------------------
// Forward Declaration
//----------------------------------
namespace paperback::coordinator
{
    struct instance;
}

//----------------------------------
// Files
//----------------------------------
#include "paperback_types.h"
#include "paperback_settings.h"
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
#include "Details/paperback_component_inline.h"
#include "Details/paperback_system_inline.h"
#include "Details/paperback_pool_inline.h"
#include "Details/paperback_entity_inline.h"
#include "Details/paperback_query_inline.h"
#include "Details/paperback_bitset_inline.h"
#include "Details/paperback_event_inline.h"
#include "Details/paperback_coordinator_inline.h"

#endif















// #ifndef PAPERBACK_H
// #define PAPERBACK_H
// #pragma once

// // Utility
// #include <memory>
// #include <thread>
// #include <utility>
// #include <cstdint>
// #include <iostream>
// #include <algorithm>
// #include <functional>
// #include <type_traits>

// // Containers
// #include <set>
// #include <map>
// #include <stack>
// #include <queue>
// #include <array>
// #include <tuple>
// #include <string>
// #include <vector>
// #include <unordered_set>
// #include <unordered_map>

// #define NOMINMAX
// #include <Windows.h>

// // Dependencies
// #include "../dependencies/xcore/src/xcore.h"

// // Files
// #include "Component.h"

// // Inline Files
// #include "Paperback_Component_Inline.h"

// #endif