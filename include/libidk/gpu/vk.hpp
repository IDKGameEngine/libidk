#pragma once

// #define VK_NO_PROTOTYPES
#include <volk/volk.h>

#include <cstdio>
#include <cassert>


#ifdef IDK_DEBUG
    #define VK_CHECK(call) \
    do { \
        VkResult __result = call; \
        if (__result != VK_SUCCESS) \
        { \
            fprintf(stderr, "Vulkan error: at %s:%d\n", __FILE__, __LINE__); \
            assert(__result == VK_SUCCESS); \
        } \
    } while (0)

#else
    #define VK_CHECK(call) call

#endif

/*
#ifdef IDK_DEBUG
    #define VK_CHECK(call) \
    do { \
        VkResult __result = call; \
        if (__result != VK_SUCCESS) \
        { \
            fprintf( \
                stderr, "Vulkan error: %s (%d) at %s:%d\n", \
                vkResultToString(__result), (int)__result, __FILE__, __LINE__\
            ); \
            assert(__result == VK_SUCCESS); \
        } \
    } while (0)

#else
    #define VK_CHECK(call) call

#endif
*/
