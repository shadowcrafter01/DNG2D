# DNG2D

Dingus 2d game engine utilizing SDL3 and box2d

includes SDL3's satellite libraries (image, ttf, mixer, etc...) and nlohmann's json library as well

## CMake Build Process

your project should call this library using CMake's FetchContent feature, intended to be built with gcc via Msys2 Mingw64

### The Following Libs are Required:
(some might be redundant now, i'll have to go through them sometime)

pacman -S mingw-w64-x86_64-gcc

pacman -S mingw-w64-x86_64-gdb

pacman -S mingw-w64-x86_64-toolchain

pacman -S mingw-w64-x86_64-ninja

pacman -S mingw-w64-x86_64-make

pacman -S mingw-w64-x86_64-cmake

pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-extra-cmake-modules

pacman -S mingw-w64-x86_64-freetype

### Example CMakeLists.txt for a project called "game":

    cmake_minimum_required(VERSION 3.24)
    project(game VERSION 0.1.0 LANGUAGES C CXX)

    # some var for vscode may or may not do anything lol
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    include(FetchContent)

    set(ENG2D_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        ENG2D
        GIT_REPOSITORY https://github.com/shadowcrafter01/DNG2D.git
        GIT_TAG main
        GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(ENG2D)

    add_executable(game src/main.cpp)

    target_compile_features(
        game
        PRIVATE
            cxx_std_20
    )

    target_link_libraries(
        game
        PRIVATE
            ENG2D::ENG2D
    )

    # use this to copy your assets folder to the build directory so the built program runs
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

    # copies the required .dll files to the build directory so the built .exe can actually run
    if(WIN32)

        add_custom_command(
            TARGET game
            POST_BUILD

            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:SDL3::SDL3>"
                "$<TARGET_FILE_DIR:game>"

            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:SDL3_image::SDL3_image>"
                "$<TARGET_FILE_DIR:game>"

            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:SDL3_ttf::SDL3_ttf>"
                "$<TARGET_FILE_DIR:game>"

            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:SDL3_mixer::SDL3_mixer>"
                "$<TARGET_FILE_DIR:game>"
        )

    endif()

    # links some stuff that lets the .exe run on machines without gcc installed
    if(MINGW)
        target_link_options(game PRIVATE
            -static
            -static-libgcc
            -static-libstdc++
        )
    endif()

### Example CMakePresets.json:
(when you use CMake to generate a template project it will want a preset, go with a compiler generated preset for mingw64 then make the preset look like this)

    {
        "version": 6,
        "cmakeMinimumRequired": {
            "major": 3,
            "minor": 25,
            "patch": 0
        },
        "configurePresets": [
            {
                "name": "mingw-build",
                "displayName": "build with mingw64",
                "description": "Using compilers: C = C:\\msys64\\mingw64\\bin\\gcc.exe, CXX = C:\\msys64\\mingw64\\bin\\g++.exe",
                "generator": "Ninja",
                "binaryDir": "${sourceDir}/build/${presetName}",
                "cacheVariables": {
                    "CMAKE_C_COMPILER": "C:/msys64/mingw64/bin/gcc.exe",
                    "CMAKE_CXX_COMPILER": "C:/msys64/mingw64/bin/g++.exe",
                    "CMAKE_EXPORT_COMPILE_COMMANDS": "ON",
                    "CMAKE_BUILD_TYPE": "Debug"
                }
            }
        ],
        "buildPresets": [
            {
                "name": "mingw-build",
                "configurePreset": "mingw-build"
            }
        ]
    }
