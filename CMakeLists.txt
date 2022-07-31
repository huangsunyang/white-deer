cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_VERBOSE_MAKEFILE on)

project(engine VERSION 0.1.0)

include(CTest)
enable_testing()

# include path
include_directories(${CMAKE_SOURCE_DIR}/src)

# src files
file(GLOB_RECURSE all_cpp_files src/*.cpp)
add_executable(engine ${all_cpp_files})

# c++17
target_compile_features(engine PRIVATE cxx_std_17)

# glfw3
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(engine PRIVATE glfw)

# gl3w
find_package(gl3w CONFIG REQUIRED)
target_link_libraries(engine PRIVATE unofficial::gl3w::gl3w)

# imgui
find_package(imgui CONFIG REQUIRED)
target_link_libraries(engine PRIVATE imgui::imgui)

# opengl
# target_link_libraries(engine PRIVATE opengl32)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
