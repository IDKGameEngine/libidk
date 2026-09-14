#pragma once

#include "IBackendContext.hpp"


namespace idk
{
    class IFilesystemBackend: public idk::IBackendFeature
    {
    private:
        
    public:
        IFilesystemBackend() {  };
        virtual size_t getFileSize(const char *filepath) = 0;
        virtual size_t loadFile(const char *filepath, void *buf, size_t bufsz) = 0;

    };
}
