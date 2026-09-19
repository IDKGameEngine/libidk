#include "libidk/platform-sdl3/FilesystemBackend.hpp"
#include "libidk/platform-sdl3/PlatformContext.hpp"
#include "libidk/Assert.hpp"

#include <SDL3/SDL.h>
#include <filesystem>

static std::FILE *fsOpenFile(const char *path, const char *mode)
{
    std::FILE *fh = std::fopen(path, mode);
    IDK_ASSERT(fh != NULL, "Failure opening file \"{}\"", path);
    return fh;
}

static void fsCloseFile(std::FILE *fh)
{
    std::fclose(fh);
}


idk::FilesystemBackend::FilesystemBackend()
{
    namespace fs = std::filesystem;
    fs::current_path(fs::path(SDL_GetBasePath()) / fs::path(IDK_CONTENT_BASEPATH));
}

size_t idk::FilesystemBackend::getFileSize(const char *filepath)
{
    std::FILE *fh = nullptr;
    size_t sz = 0;

    fh = fsOpenFile(filepath, "r");
    std::fseek(fh, 0, SEEK_END);
    sz = std::ftell(fh);
    fsCloseFile(fh);

    return sz;
}

size_t idk::FilesystemBackend::loadFile(const char *filepath, void *buf, size_t bufsz)
{
    std::FILE *fh = fsOpenFile(filepath, "r");
    std::fseek(fh, 0, SEEK_END);
    size_t filesize = std::ftell(fh);

    if (filesize > bufsz)
    {
        std::fclose(fh);
        return 0;
    }

    std::fseek(fh, 0, SEEK_SET);
    IDK_ASSERT(filesize == std::fread(buf, 1, filesize, fh), "Failure loading \"{}\"", filepath);
    fsCloseFile(fh);

    return filesize;
}
