cmake_minimum_required(VERSION 3.0.0)
project(engine VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(engine main.cpp)

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
target_link_libraries(engine PRIVATE opengl32)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
