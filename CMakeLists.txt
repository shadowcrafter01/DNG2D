cmake_minimum_required(VERSION 3.20.0)
project(
    DNG2D
    #VERSION 0.1.0
    LANGUAGES C CXX
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # lets VS Code's IntelliSense find everything

option(DNG2D_BUILD_TESTS "Build DNG2D tests" OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/bin"
)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/lib"
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
    "${CMAKE_BINARY_DIR}/lib"
)

include(GNUInstallDirs)
include(FetchContent)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(SDL_SHARED OFF CACHE BOOL "" FORCE)
set(SDL_STATIC ON CACHE BOOL "" FORCE)

# SDL3
set(SDL_TESTS OFF CACHE BOOL "" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG "release-3.4.18"
    GIT_SHALLOW TRUE
)

# SDL_image
set(SDLIMAGE_TESTS OFF CACHE BOOL "" FORCE)
set(SDLIMAGE_SAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SDL3_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG "release-3.4.8"
    GIT_SHALLOW TRUE
)

# SDL_ttf
set(SDLTTF_VENDORED ON CACHE BOOL "" FORCE)
set(SDLTTF_TESTS OFF CACHE BOOL "" FORCE)
set(SDLTTF_SAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SDL3_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG "release-3.2.2"
    GIT_SHALLOW TRUE
)

# SDL_mixer
set(SDLMIXER_VENDORED ON CACHE BOOL "" FORCE)
set(SDLMIXER_TESTS OFF CACHE BOOL "" FORCE)
set(SDLMIXER_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    SDL3_mixer
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
    GIT_TAG "release-3.2.2"
    GIT_SHALLOW TRUE
)

# SDL_net
FetchContent_Declare(
    SDL3_net
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_net.git
    GIT_TAG "release-3.2.0"
    GIT_SHALLOW TRUE
)

# Box2D
set(BOX2D_SAMPLES OFF CACHE BOOL "" FORCE)
set(BOX2D_UNIT_TESTS OFF CACHE BOOL "" FORCE)
set(BOX2D_DOCS OFF CACHE BOOL "" FORCE)
set(BOX2D_BENCHMARKS OFF CACHE BOOL "" FORCE)
#set(BOX2D_DOUBLE_PRECISION OFF)
FetchContent_Declare(
    box2d
    GIT_REPOSITORY https://github.com/erincatto/box2d.git
    GIT_TAG main
    GIT_SHALLOW TRUE
)

# nlohmann json

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG "v3.12.0"
    GIT_SHALLOW TRUE
)


FetchContent_MakeAvailable(
    SDL3
    SDL3_image
    SDL3_ttf
    SDL3_mixer
    SDL3_net
    box2d
    json
)

add_library(DNG2D INTERFACE)
add_library(DNG2D::DNG2D ALIAS DNG2D)

target_compile_features(
    DNG2D
    INTERFACE
    cxx_std_20
)

target_include_directories(
    DNG2D
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(
    DNG2D
    INTERFACE
        SDL3::SDL3
        SDL3_image::SDL3_image
        SDL3_ttf::SDL3_ttf
        SDL3_mixer::SDL3_mixer
        box2d::box2d
        nlohmann_json::nlohmann_json
)

if(DNG2D_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()