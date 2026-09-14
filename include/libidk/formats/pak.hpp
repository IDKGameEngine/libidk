#pragma once

#include <cstdint>

namespace idk::format
{
    struct __attribute__((packed)) PakFileHeader
    {
        char signature[8]; // "idk_pak";
    };

    struct __attribute__((packed)) PakFileEntry
    {
        char name[56];
        int32_t offset;
        int32_t length;
    };


    // void sdfd()
    // {
    //     sizeof(PakFileEntry);
    // }
}
