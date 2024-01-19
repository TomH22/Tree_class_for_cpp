cmake_minimum_required(VERSION 3.19.2)
project(Tree_class_for_cpp CXX)

add_subdirectory(extern/json)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    message(FATAL_ERROR "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there. You may need to remove CMakeCache.txt. ")
  endif()

set(CMAKE_CXX_STANDARD 20 CACHE STRING "CMAKE_CXX_STANDARD")
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "CMAKE_CONFIGURATION_TYPES")
set(CMAKE_INSTALL_PREFIX "C:/Program Files/${PROJECT_NAME}" CACHE PATH "CMAKE_INSTALL_PREFIX ")

enable_testing()

add_library(lib INTERFACE)

target_include_directories(lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_include_directories(lib INTERFACE $<TARGET_PROPERTY:nlohmann_json::nlohmann_json,INTERFACE_INCLUDE_DIRECTORIES>)

# Example glfw app just tested for windows!
if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux" AND (NOT DEFINED ENV{DISPLAY} OR "$ENV{TERM}" STREQUAL "linux"))
    message(STATUS "No X Windows System on Linux.")
else()
    # Set this to point to an up-to-date GLFW repo.
    set(GLFW_DIR "${PROJECT_SOURCE_DIR}/../glfw")

    # Set this to point to an up-to-date imgui repo.
    set(IMGUI_DIR "${PROJECT_SOURCE_DIR}/../imgui")

    find_package(Vulkan)

    if(EXISTS ${GLFW_DIR} AND EXISTS ${IMGUI_DIR} AND ${Vulkan_FOUND})
        add_subdirectory(src bin)
    else()
        if(NOT EXISTS ${GLFW_DIR})
            message(WARNING "GLFW not repository found: ${GLFW_DIR}")
        endif()
        if(NOT EXISTS ${IMGUI_DIR})
            message(WARNING "Imgui not repository found: ${IMGUI_DIR}")
        endif()
        if(NOT ${Vulkan_FOUND})
            message(WARNING "Vulkan not found. Install it!")
        endif()        

    endif()
endif()

add_subdirectory(test)