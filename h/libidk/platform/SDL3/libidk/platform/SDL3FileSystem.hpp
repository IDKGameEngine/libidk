#pragma once

#include "libidk/platform/IPlatformFileSystem.hpp"


namespace idk
{
    class SDL3FileSystem: public IPlatformFileSystem
    {
    private:
        
    public:
        SDL3FileSystem();
        virtual size_t getFileSize(const char *filepath) final;
        virtual size_t loadFile(const char *filepath, void *buf, size_t bufsz) final;
    };
}
