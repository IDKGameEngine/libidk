#pragma once

#ifdef __GNUC__
    #define idk_strcat __builtin_strcat
    #define idk_strchr __builtin_strchr
    #define idk_strcmp __builtin_strcmp
    #define idk_strcpy __builtin_strcpy
    #define idk_strcspn __builtin_strcspn
    #define idk_strlen __builtin_strlen
    #define idk_strncat __builtin_strncat
    #define idk_strncmp __builtin_strncmp
    #define idk_strncpy __builtin_strncpy
    #define idk_strpbrk __builtin_strpbrk
    #define idk_strrchr __builtin_strrchr
    #define idk_strspn __builtin_strspn
    #define idk_strstr __builtin_strstr
#else
    #include <cstring>
    #define idk_strcat strcat
    #define idk_strchr strchr
    #define idk_strcmp strcmp
    #define idk_strcpy strcpy
    #define idk_strcspn strcspn
    #define idk_strlen strlen
    #define idk_strncat strncat
    #define idk_strncmp strncmp
    #define idk_strncpy strncpy
    #define idk_strpbrk strpbrk
    #define idk_strrchr strrchr
    #define idk_strspn strspn
    #define idk_strstr strstr
#endif
