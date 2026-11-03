# ENG2D
2d c++ game engine built on SDL3 and Box2D

includes SDL3's satellite libraries (image, ttf, mixer, etc...) and nlohmann's json library as well

## CMake Build Process

your project should call this library using CMake's FetchContent feature, intended to be built with gcc via Msys2 Mingw64

### you'll need the following libs installed:
(some might be redundant now, i'll have to go through them sometime)

pacman -S mingw-w64-x86_64-gcc

pacman -S mingw-w64-x86_64-gdb

pacman -S mingw-w64-x86_64-toolchain

pacman -S mingw-w64-x86_64-ninja

pacman -S mingw-w64-x86_64-make

pacman -S mingw-w64-x86_64-cmake

pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-extra-cmake-modules

pacman -S mingw-w64-x86_64-freetype

### And Here's an Example CMakeLists.txt for a project called "game":

    cmake_minimum_required(VERSION 3.24)
    project(game VERSION 0.1.0 LANGUAGES C CXX)

    # some var for vscode may or may not do anything lol
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    include(FetchContent)

    set(ENG2D_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        ENG2D
        GIT_REPOSITORY https://github.com/shadowcrafter01/ENG2D.git
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