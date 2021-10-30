# Minimum
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(HMCD VERSION 2.0.0)


# Define global paths
set(HMCD_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}" CACHE INTERNAL "")
set(HMCD_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src" CACHE INTERNAL "")
set(HMCD_INC_DIR "${CMAKE_CURRENT_LIST_DIR}/include" CACHE INTERNAL "")
set(HMCD-Core_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src/hmcdcore" CACHE INTERNAL "")
set(HMCD-Core_INC_DIR "${CMAKE_CURRENT_LIST_DIR}/include/hmcdcore" CACHE INTERNAL "")
set(HMCD-CLI_SRC_DIR "${CMAKE_CURRENT_LIST_DIR}/src/hmcd-cli" CACHE INTERNAL "")
string(REPLACE "/" "\\" HMCD_ROOT_DIR_WIN "${HMCD_ROOT_DIR}\\")

# Macro definitions
if (CMAKE_BUILD_TYPE MATCHES "Debug")
    message("Building Debug Version")
else()
    message("Building Release Version")
    set(RELEASE_BUILD 1 CACHE INTERNAL "")
    add_definitions("-DRELEASE_BUILD=${RELEASE_BUILD}")
endif()

# Flags
set(HMCD_FLAGS "-Wall" "-O3" CACHE INTERNAL "")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    list(APPEND HMCD_FLAGS "-Wextra")
endif()

if (RELEASE_BUILD EQUAL 1)
    if (WIN32)
        list(APPEND HMCD_FLAGS "-ffile-prefix-map=${HMCD_ROOT_DIR_WIN}=")
    else()
        list(APPEND HMCD_FLAGS "-ffile-prefix-map=${HMCD_ROOT_DIR}=")
    endif()
else()
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        list(APPEND HMCD_FLAGS "-ggdb")
    else()
        list(APPEND HMCD_FLAGS "-g3")
    endif()
endif()

# Documentation
find_package(Doxygen)
option(BUILD_DOCUMENTATION "Create and install the HTML based API
documentation (requires Doxygen)" "${DOXYGEN_FOUND}")

# https://stackoverflow.com/questions/34878276/build-doxygen-from-cmake-script
if(BUILD_DOCUMENTATION)
    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation")
    endif()

    file(RELATIVE_PATH RELATIVE_BINARY_DIR "${HMCD_ROOT_DIR}" "${CMAKE_CURRENT_BINARY_DIR}")
    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in" "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile")

    message("Doxygen build started")
    add_custom_target(docs
        COMMAND "${DOXYGEN_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile"
        WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

    install(DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/docs/html" DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/HMCD")
endif()

# Build HMCD
include("${HMCD-CLI_SRC_DIR}/CMakeLists.txt")

# From https://gitlab.kitware.com/cmake/community/-/wikis/FAQ#can-i-do-make-uninstall-with-cmake
# uninstall target
if (NOT TARGET uninstall)
    configure_file(
        "${CMAKE_CURRENT_LIST_DIR}/cmake_uninstall.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
        IMMEDIATE @ONLY)

    add_custom_target(uninstall
        COMMAND "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")
endif()
