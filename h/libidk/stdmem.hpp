#pragma once

#ifdef __GNUC__
    #define idk_memchr __builtin_memchr
    #define idk_memcmp __builtin_memcmp
    #define idk_memcpy __builtin_memcpy
    #define idk_memset __builtin_memset
#else
    #include <cstring>
    #define idk_memchr memchr
    #define idk_memcmp memcmp
    #define idk_memcpy memcpy
    #define idk_memset memset
#endif
