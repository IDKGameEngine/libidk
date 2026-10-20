target_sources(
    ${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/LocalRxTx.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/SharedRxTx.cpp"
)

option(IDK_BUILD_REMOTE "Build SDL_net remote messaging" ON)
if(IDK_BUILD_REMOTE)
    find_package(SDL3 REQUIRED CONFIG GLOBAL)
    find_package(SDL3_net REQUIRED CONFIG GLOBAL)
    add_library(libidk_remote STATIC RemoteRxTx.cpp)
    target_compile_features(libidk_remote PUBLIC cxx_std_23)
    target_include_directories(libidk_remote PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../../include")
    target_link_libraries(libidk_remote PUBLIC libidk PRIVATE SDL3_net::SDL3_net SDL3::SDL3)
    target_compile_options(libidk_remote PRIVATE -Wall -Wextra -Werror -fno-exceptions)
endif()
