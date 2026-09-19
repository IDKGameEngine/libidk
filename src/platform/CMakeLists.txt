target_sources(
    ${PROJECT_NAME} PRIVATE "IPlatformContext.cpp"
)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/platform-sdl3")
