cmake_minimum_required(VERSION 4.2)

project(libidk VERSION 0.0.1)

add_library(
    ${PROJECT_NAME} STATIC
)
target_compile_features(
    ${PROJECT_NAME} PUBLIC cxx_std_23
)
target_compile_options(
    ${PROJECT_NAME} PUBLIC -Werror -Wall -Wextra
)
target_link_libraries(
    ${PROJECT_NAME} PUBLIC glad SDL3::SDL3-shared SDL3_net::SDL3_net glm::glm-header-only
)
target_include_directories(
    ${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/h"
)
target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/assert.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/camera.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cfgparser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/file.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/log.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Service.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/threadpool.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/transform.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/UdpMessage.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/detail/AllocatorImpl.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/platform/PlatformSDL3.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/platform/PlatformSDL3GL.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/platform/WindowSDL3.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/platform/WindowSDL3GL.cpp"
)
