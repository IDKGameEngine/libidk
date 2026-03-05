cmake_minimum_required(VERSION 3.5)

project(libidk VERSION 0.0.1)
# add_compile_options(-Werror -Wall -Wextra)

add_library(
    ${PROJECT_NAME} OBJECT
)
target_compile_features(
    ${PROJECT_NAME} PUBLIC cxx_std_23
)
target_include_directories(
    ${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/h"
)
target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/assert.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/log.cpp"
)
