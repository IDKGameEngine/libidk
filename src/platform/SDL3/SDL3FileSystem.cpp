#include "libidk/platform/SDL3FileSystem.hpp"
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



idk::SDL3FileSystem::SDL3FileSystem()
{
    namespace fs = std::filesystem;
    fs::current_path(fs::path(SDL_GetBasePath()) / fs::path("data"));
}

size_t idk::SDL3FileSystem::getFileSize(const char *filepath)
{
    std::FILE *fh = nullptr;
    size_t sz = 0;

    fh = fsOpenFile(filepath, "r");
    std::fseek(fh, 0, SEEK_END);
    sz = std::ftell(fh);
    fsCloseFile(fh);

    return sz;
}

size_t idk::SDL3FileSystem::loadFile(const char *filepath, void *buf, size_t bufsz)
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
