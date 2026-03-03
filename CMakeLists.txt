cmake_minimum_required(VERSION 3.15)
project(FileManager)

# ── Standard C++17 ──────────────────────────────────────────
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Include headers ──────────────────────────────────────────
include_directories(include)

# ── MSVC specific settings ───────────────────────────────────
if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# ── Sources ──────────────────────────────────────────────────
add_executable(file_manager
    main.cpp    src/FileHandle.cpp
    src/File.cpp
    src/Directory.cpp
    src/FileSystem.cpp)

# ── Warnings ─────────────────────────────────────────────────
if(MSVC)
    target_compile_options(file_manager PRIVATE /W4)
else()
    target_compile_options(file_manager PRIVATE
        -Wall
        -Wextra
        -Wpedantic
    )
endif()