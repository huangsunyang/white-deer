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

# test target needs remove main.cpp
list(FILTER all_cpp_files EXCLUDE REGEX ".*main.cpp")
file(GLOB_RECURSE all_test_files test/*.cpp)
add_executable(test-engine ${all_cpp_files} ${all_test_files})

# c++17
target_compile_features(test-engine PRIVATE cxx_std_17)

# glfw3
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(test-engine PRIVATE glfw)

# gl3w
find_package(gl3w CONFIG REQUIRED)
target_link_libraries(test-engine PRIVATE unofficial::gl3w::gl3w)

# imgui
find_package(imgui CONFIG REQUIRED)
target_link_libraries(test-engine PRIVATE imgui::imgui)

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(test-engine PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

# use catch discover tests
set(VCPKG_INSTALL_ROOT ${CMAKE_TOOLCHAIN_FILE}/../../../installed)
set(CATCH_SRC_DIR ${VCPKG_INSTALL_ROOT}/${VCPKG_TARGET_TRIPLET}/Catch2)
list(APPEND CMAKE_MODULE_PATH ${CATCH_SRC_DIR})
include(Catch)
catch_discover_tests(test-engine)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
