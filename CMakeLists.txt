cmake_minimum_required(VERSION 3.10.0)
project(ImageRoamer VERSION 0.1.0 LANGUAGES C CXX)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG)
endif()

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/source-charset:utf-8>")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL2 CONFIG REQUIRED)
find_package(GTest REQUIRED)
find_package(OpenCLHeaders REQUIRED)
find_package(OpenCLICDLoader REQUIRED)
find_package(OpenCLHeadersCpp REQUIRED)

include_directories("include")
include_directories(3rdparty/imgui)
include_directories(3rdparty/stb)
file(GLOB SOURCES src/*.cpp)
file(GLOB IMGUI_SOURCES 3rdparty/imgui/*.cpp)

add_library(Imgui ${IMGUI_SOURCES})
target_link_libraries(Imgui PRIVATE SDL2::SDL2-static SDL2::SDL2main)

add_library(ImageRoamer STATIC ${SOURCES})
target_link_libraries(ImageRoamer PRIVATE SDL2::SDL2-static SDL2::SDL2main)
target_link_libraries(ImageRoamer PRIVATE OpenCL::Headers OpenCL::OpenCL OpenCL::HeadersCpp)
target_compile_definitions(ImageRoamer PRIVATE CL_HPP_TARGET_OPENCL_VERSION=300)

if (WIN32)
    if ("${CMAKE_MAKE_PROGRAM}" MATCHES "MsBuild")
        add_custom_command(TARGET ImageRoamer
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/$(ConfigurationName)/assets
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/kernel ${CMAKE_BINARY_DIR}/$(ConfigurationName)/kernel
                )
    elseif(MINGW)
        add_custom_command(TARGET ImageRoamer
                POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/kernel ${CMAKE_BINARY_DIR}/kernel
                )
    endif ()
endif ()

install(TARGETS ImageRoamer RUNTIME DESTINATION bin)

add_executable(ImageRoamerExample example/image_roamer_example.cpp)
target_link_libraries(ImageRoamerExample PRIVATE Imgui)
target_link_libraries(ImageRoamerExample PRIVATE SDL2::SDL2-static SDL2::SDL2main)
target_link_libraries(ImageRoamerExample PRIVATE ImageRoamer)

add_executable(ImageRoamerTest test/area_segmentation_test.cpp)
target_link_libraries(ImageRoamerTest PRIVATE ${GTEST_BOTH_LIBRARIES} pthread)
target_link_libraries(ImageRoamerTest PRIVATE ImageRoamer)

gtest_discover_tests(ImageRoamerTest)