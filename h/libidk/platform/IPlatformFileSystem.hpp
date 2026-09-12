#pragma once

#include "PlatformContext.hpp"


namespace idk
{
    class IPlatformFileSystem: public IPlatformFeature
    {
    private:
        
    public:
        IPlatformFileSystem() {  };
        virtual size_t getFileSize(const char *filepath) = 0;
        virtual size_t loadFile(const char *filepath, void *buf, size_t bufsz) = 0;

    };
}
