cmake_minimum_required(VERSION 3.10)
project(Bharata VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Include + Link manually for SDL2 and SDL2_image via Homebrew
include_directories(
    /opt/homebrew/include
    /opt/homebrew/include/SDL2
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/glad
    ${CMAKE_SOURCE_DIR}/src/core
    ${CMAKE_SOURCE_DIR}/src/grid
    ${CMAKE_SOURCE_DIR}/src/ai
    ${CMAKE_SOURCE_DIR}/src/texture
    ${CMAKE_SOURCE_DIR}/src/unit
)

link_directories(/opt/homebrew/lib)

# Find OpenGL
find_package(OpenGL REQUIRED)

# GLAD
add_library(glad src/glad/glad.c)
target_include_directories(glad PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Executable
add_executable(Bharata
    src/main.cpp
    src/core/game.cpp
    src/grid/grid.cpp
    src/ai/ai.cpp
    src/texture/texture.cpp
    src/unit/unit.cpp
)

# Link SDL2, SDL2_image, OpenGL, GLAD
target_link_libraries(Bharata
  PRIVATE
    glad
    SDL2
    SDL2_image
    ${OPENGL_LIBRARIES}
)
