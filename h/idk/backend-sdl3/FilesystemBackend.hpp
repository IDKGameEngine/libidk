#pragma once

#include "idk/backend/IFilesystemBackend.hpp"


namespace idk
{
    class FilesystemBackend: public idk::IFilesystemBackend
    {
    private:
        
    public:
        FilesystemBackend();
        virtual size_t getFileSize(const char *filepath) final;
        virtual size_t loadFile(const char *filepath, void *buf, size_t bufsz) final;
    };
}
