#include "libidk/platform/SDL3FileSystem.hpp"
#include <SDL3/SDL.h>
#include <filesystem>

idk::SDL3FileSystem::SDL3FileSystem()
{
    namespace fs = std::filesystem;
    fs::current_path(fs::path(SDL_GetBasePath()) / fs::path(IDK_ASSETS_DIRNAME));
}
