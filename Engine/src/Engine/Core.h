#pragma once

#include "Log.h"

#ifdef DEBUG
#ifdef WINDOWS
#define ASSERT(a)                                                                                                      \
    if (!a)                                                                                                            \
    {                                                                                                                  \
        CORE_ERROR("ASSERT FAILED FILE:{0}, LINE:{1}", __FILE__, __LINE__);                                            \
        __debugbreak();                                                                                                \
    }
#endif

#ifdef LINUX
#define ASSERT(a)                                                                                                      \
    if (!a)                                                                                                            \
    {                                                                                                                  \
        CORE_ERROR("ASSERT FAILED FILE:{0}, LINE:{1}", __FILE__, __LINE__);                                            \
        __builtin_trap();                                                                                              \
    }

#endif

#endif

#ifdef RELEASE
#define ASSERT(a)
#endif
