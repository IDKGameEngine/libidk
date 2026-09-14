add_library(
    idkplatform OBJECT
)
target_compile_features(
    idkplatform PRIVATE cxx_std_23
)
target_compile_options(
    idkplatform PRIVATE -Werror -Wall -Wextra
)
target_include_directories(
    idkplatform PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
target_sources(
    idkplatform PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/AudioBackend.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/EventBackend.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/FilesystemBackend.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/InputBackend.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/PlatformContext.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/TimeBackend.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/VideoBackend.cpp"
)
