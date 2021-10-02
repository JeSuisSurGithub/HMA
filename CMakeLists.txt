# Minimum
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(HMCD VERSION 1.3.0)

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Define paths
set(HMCD_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}" CACHE INTERNAL "")
set(HMCD-Core_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src/HMCDCore" CACHE INTERNAL "")
set(HMCD-Core_INC_DIR "${CMAKE_CURRENT_LIST_DIR}/include/HMCDCore" CACHE INTERNAL "")
set(HMCD-CLI_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src/HMCD-CLI" CACHE INTERNAL "")

# Build HMCD
include("${HMCD-CLI_SRC_DIR}/CMakeLists.txt")

# From https://gitlab.kitware.com/cmake/community/-/wikis/FAQ#can-i-do-make-uninstall-with-cmake
# uninstall target
if(NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_LIST_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY)

    add_custom_target(uninstall
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()

