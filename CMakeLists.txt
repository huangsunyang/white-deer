cmake_minimum_required(VERSION 3.0.0)
set(CMAKE_VERBOSE_MAKEFILE on)
set(VCPKG_INSTALL_ROOT ${CMAKE_TOOLCHAIN_FILE}/../../../installed)

project(engine VERSION 0.1.0)

include(CTest)
enable_testing()

if(MSVC)
    add_compile_options(/Zc:preprocessor)
endif()

# include path
include_directories(${CMAKE_SOURCE_DIR}/src)
include_directories(${CMAKE_SOURCE_DIR}/src/shaderlab/generated)
include_directories(${CMAKE_SOURCE_DIR}/third-party/antlr4-cpp-runtime-4.12.0-source/runtime/src)

# self managed 3rd party libraries
add_subdirectory(${CMAKE_SOURCE_DIR}/third-party/antlr4-cpp-runtime-4.12.0-source)


find_package(glfw3 CONFIG REQUIRED)
find_package(gl3w CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(plog CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(Tracy CONFIG REQUIRED)
find_package(Lua REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(rttr CONFIG REQUIRED)
find_package(sol2 CONFIG REQUIRED)
find_path(STB_INCLUDE_DIRS "stb_c_lexer.h")

macro(apply_common_config target_name)
    # c++17
    target_compile_features(${target_name} PRIVATE cxx_std_17)
    target_include_directories(${target_name} PRIVATE ${LUA_INCLUDE_DIR} ${STB_INCLUDE_DIRS})

    target_link_libraries(${target_name} PRIVATE
        glfw
        glm::glm
        fmt::fmt
        RTTR::Core
        unofficial::gl3w::gl3w
        imgui::imgui
        plog::plog
        Tracy::TracyClient
        sol2
        ${LUA_LIBRARIES}
    )

    add_dependencies(${target_name} antlr4_static)
    target_link_libraries(${target_name} PRIVATE antlr4_static)
    # add_custom_command(TARGET ${target_name} POST_BUILD
        # COMMAND ${CMAKE_COMMAND} -E copy_if_different
        # $<TARGET_FILE:antlr4_shared> $<TARGET_FILE_DIR:${target_name}>
    # )

    # opengl
    # target_link_libraries(${target_name} PRIVATE opengl32)
endmacro(apply_common_config)

# src files
file(GLOB_RECURSE all_cpp_files src/*.cpp)
add_executable(engine ${all_cpp_files})
apply_common_config(engine)

# test target needs remove main.cpp
list(FILTER all_cpp_files EXCLUDE REGEX ".*main.cpp")
file(GLOB_RECURSE all_test_files test/*.cpp)
add_executable(test-engine ${all_cpp_files} ${all_test_files})
apply_common_config(test-engine)

# for test target only
find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(test-engine PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

# use catch discover tests
set(CATCH_SRC_DIR ${VCPKG_INSTALL_ROOT}/${VCPKG_TARGET_TRIPLET}/Catch2)
list(APPEND CMAKE_MODULE_PATH ${CATCH_SRC_DIR})
include(Catch)
catch_discover_tests(test-engine)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
