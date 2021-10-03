# Minimum
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(HMCD VERSION 1.3.0)

# Macro definitions
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    message("Building Debug Version")
else()
    message("Building Release Version")
    set(RELEASE_BUILD 1 CACHE INTERNAL "")
    add_definitions("-DRELEASE_BUILD=${RELEASE_BUILD}")
endif()

# Define paths
set(HMCD_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}" CACHE INTERNAL "")
set(HMCD_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src" CACHE INTERNAL "")
set(HMCD_INC_DIR "${CMAKE_CURRENT_LIST_DIR}/include" CACHE INTERNAL "")
set(HMCD-Core_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src/HMCDCore" CACHE INTERNAL "")
set(HMCD-Core_INC_DIR "${CMAKE_CURRENT_LIST_DIR}/include/HMCDCore" CACHE INTERNAL "")
set(HMCD-CLI_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src/HMCD-CLI" CACHE INTERNAL "")


# Documentation
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API
documentation (requires Doxygen)" "${DOXYGEN_FOUND}")

if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation")
    endif()

    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in" "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile")

    message("Doxygen build started")
    add_custom_target(docs
        COMMAND "${DOXYGEN_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/html" DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc")
endif()

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
        COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
endif()
