find_package(SDL3 CONFIG REQUIRED)

add_library(idk_platform_sdl3 STATIC "${CMAKE_CURRENT_SOURCE_DIR}/SdlPlatform.cpp")
add_library(idk::platform_sdl3 ALIAS idk_platform_sdl3)

target_compile_features(idk_platform_sdl3 PUBLIC cxx_std_23)
target_include_directories(idk_platform_sdl3 PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../include")
target_link_libraries(idk_platform_sdl3 PUBLIC libidk PRIVATE SDL3::SDL3)
