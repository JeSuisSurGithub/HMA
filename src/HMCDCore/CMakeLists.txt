cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(HMCD-Core VERSION 2.0.0)

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Macro definitions
if (CMAKE_BUILD_TYPE MATCHES "Release")
    set(RELEASE_BUILD 1)
else()
    set(RELEASE_BUILD 0)
endif()

#get_filename_component(ABSOLUTE_PROJECT_PATH "${CMAKE_CURRENT_LIST_DIR}/../../" ABSOLUTE)
string(LENGTH "${HMCD_ROOT_DIR}/" SRC_PATH_LEN)
add_definitions("-DSRC_PATH_LEN=${SRC_PATH_LEN}")

# Configure version
configure_file("${HMCD-Core_INC_DIR}/HMCD-Core_Version.h.in" "${HMCD-Core_INC_DIR}/HMCD-Core_Version.h")

# List of files to compile
set(HMCD-Core_SRC_LIST
    "${HMCD-Core_SRC_DIR}/HMCD.c"
    "${HMCD-Core_SRC_DIR}/DirScanUtil.c"
)

# Build shared and static
add_library(HMCD-Core_static STATIC "${HMCD-Core_SRC_LIST}")
add_library(HMCD-Core_shared SHARED "${HMCD-Core_SRC_LIST}")

# Includes
target_include_directories(HMCD-Core_static PUBLIC "${CMAKE_CURRENT_LIST_DIR}" "${HMCD-Core_INC_DIR}/../")
target_include_directories(HMCD-Core_shared PUBLIC "${CMAKE_CURRENT_LIST_DIR}" "${HMCD-Core_INC_DIR}/../")

# Common output name
set_target_properties(HMCD-Core_static PROPERTIES OUTPUT_NAME hmcd)
set_target_properties(HMCD-Core_shared PROPERTIES OUTPUT_NAME hmcd)

# Search libraries
find_package(CURL)
if(CURL_FOUND)
    include_directories("${CURL_INCLUDE_DIR}")
    target_link_libraries(HMCD-Core_static "${CURL_LIBRARIES}")
    target_link_libraries(HMCD-Core_shared "${CURL_LIBRARIES}")
else (CURL_FOUND)
    message(FATAL_ERROR "cURL developement package hasn't been installed")
endif(CURL_FOUND)

# Flags
target_compile_options(HMCD-Core_static PUBLIC "-Wall" "-O2")
target_compile_options(HMCD-Core_shared PUBLIC "-Wall" "-O2")

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(HMCD-Core_static PUBLIC "-Wextra")
    target_compile_options(HMCD-Core_shared PUBLIC "-Wextra")
endif()

# Install
install(TARGETS HMCD-Core_static DESTINATION "${CMAKE_INSTALL_LIBDIR}")
install(TARGETS HMCD-Core_shared DESTINATION "${CMAKE_INSTALL_LIBDIR}")
install(DIRECTORY "${HMCD-Core_INC_DIR}" DESTINATION "${CMAKE_INSTALL_PREFIX}/include")