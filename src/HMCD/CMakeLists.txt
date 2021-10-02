cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(HMCD VERSION 1.3.0)

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Define paths
set(HMCD_src_prefix "${CMAKE_CURRENT_LIST_DIR}/")
set(HMCD_inc_prefix "${CMAKE_CURRENT_LIST_DIR}/../../include")

# Macro definitions
if (CMAKE_BUILD_TYPE MATCHES "Release")
    set(RELEASE_BUILD 1)
else()
    set(RELEASE_BUILD 0)
endif()

get_filename_component(ABSOLUTE_PROJECT_PATH "${HMCD_src_prefix}/../../" ABSOLUTE)
string(LENGTH "${ABSOLUTE_PROJECT_PATH}/" SRC_PATH_SIZE)
add_definitions("-DSRC_PATH_SIZE=${SRC_PATH_SIZE}")

# Configure version
configure_file(
    "${HMCD_inc_prefix}/HMCD/HMCD_Version.h.in" "${HMCD_inc_prefix}/HMCD/HMCD_Version.h")

# List of files to compile
set(HMCD_src_list
                "${HMCD_src_prefix}/HMCD.c"
                "${HMCD_src_prefix}/DirScanUtil.c"
)

# Build shared and static
add_library(shared_HMCD SHARED
                "${HMCD_src_list}"
)
add_library(static_HMCD STATIC
                "${HMCD_src_list}"
)

# Includes
target_include_directories(shared_HMCD PUBLIC "${CMAKE_CURRENT_LIST_DIR}" "${HMCD_inc_prefix}")
target_include_directories(static_HMCD PUBLIC "${CMAKE_CURRENT_LIST_DIR}" "${HMCD_inc_prefix}")

# Libraries
target_link_libraries(shared_HMCD "curl")
target_link_libraries(static_HMCD "curl")

# Common output name
set_target_properties(shared_HMCD PROPERTIES OUTPUT_NAME hmcd)
set_target_properties(static_HMCD PROPERTIES OUTPUT_NAME hmcd)

# Flags
target_compile_options(shared_HMCD PUBLIC "-Wall" "-O2")
target_compile_options(static_HMCD PUBLIC "-Wall" "-O2")

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(static_HMCD PUBLIC "-Wextra")
    target_compile_options(shared_HMCD PUBLIC "-Wextra")
endif()
