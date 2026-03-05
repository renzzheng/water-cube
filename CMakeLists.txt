cmake_minimum_required(VERSION 3.20)
project(WaterCube)

set(CMAKE_CXX_STANDARD 17)

# Find packages
find_package(OpenGL REQUIRED)

# GLFW
set(GLFW_LIB /opt/homebrew/lib/libglfw.dylib)
set(GLFW_INCLUDE /opt/homebrew/include)

# All source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
list(APPEND SOURCES "src/glad.c")

add_executable(WaterCube ${SOURCES})

target_include_directories(WaterCube PRIVATE
    ${GLFW_INCLUDE}
    ${OPENGL_INCLUDE_DIR}
    /opt/homebrew/include
    src/
)

target_link_libraries(WaterCube PRIVATE
    ${GLFW_LIB}
    ${OPENGL_LIBRARIES}
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
)