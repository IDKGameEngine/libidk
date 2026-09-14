
target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/AudioBackend.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/EventBackend.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/FilesystemBackend.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/InputBackend.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/TimeBackend.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/VideoBackend.cpp"
)
