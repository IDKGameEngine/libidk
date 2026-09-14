
find_package(volk CONFIG REQUIRED)
# find_package(VulkanMemoryAllocator CONFIG REQUIRED)

target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/AudioBackend.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/EventBackend.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/FilesystemBackend.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/InputBackend.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/TimeBackend.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/VideoBackend.cpp"
)
target_include_directories(
    ${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/h"
)
target_compile_features(
    ${PROJECT_NAME} PUBLIC cxx_std_23
)
target_compile_options(
    ${PROJECT_NAME} PUBLIC -Werror -Wall -Wextra
)
target_link_libraries(
    ${PROJECT_NAME} PRIVATE SDL3 SDL3_image SDL3_mixer volk::volk
)
