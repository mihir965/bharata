cmake_minimum_required(VERSION 3.10)
project(Bharata VERSION 1.0)

#Setting C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#Locate SDL2
find_package(SDL2 REQUIRED)
if(SDL2_FOUND)
    include_directories(${SDL2_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "SDL2 not found")
endif()

#Locate OpenGL
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
    include_directories(${OPENGL_INCLUDE_DIRS})
else()
    message(FATAL_ERROR "OPENGL not found")
endif()

#Add GLAD
add_library(glad src/glad.c)
target_include_directories(glad PUBLIC ${PROJECT_SOURCE_DIR}/include)

#Add your executable target with your source files
add_executable(MyGame src/main/main.cpp src/Renderer.cpp src/Game.cpp src/map/map.cpp)

target_link_libraries(MyGame PRIVATE glad ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES}) 
