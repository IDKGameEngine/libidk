target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/SDL3Audio.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/SDL3Events.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/SDL3FileSystem.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/SDL3Input.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/SDL3Time.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/SDL3Video.cpp"
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC
    glad
    SDL3::SDL3-shared
    SDL3_image::SDL3_image
    SDL3_mixer::SDL3_mixer
)
