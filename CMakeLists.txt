cmake_minimum_required(VERSION 3.31)

project(libidk VERSION 0.0.1)

add_library(
    ${PROJECT_NAME} STATIC
)
set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "${PROJECT_NAME}"
    PREFIX ""
)
target_compile_features(
    ${PROJECT_NAME} PUBLIC cxx_std_23
)
target_compile_options(
    ${PROJECT_NAME} PUBLIC -Werror -Wall -Wextra
)
target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    SDL3_net
    glm::glm-header-only
)
target_include_directories(
    ${PROJECT_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/h"
    "${CMAKE_CURRENT_SOURCE_DIR}/h/libidk/platform/${IDK_PLATFORM}"
)
target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/assert.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/camera.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/cfgparser.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Config.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/file.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/log.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/New.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/Service.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/SharedMemory.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/transform.cpp"
)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/dsa")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/message")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/object")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/src/platform/${IDK_PLATFORM}")

