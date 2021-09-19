cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(HMCDCore VERSION 1.2.0)

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Define paths
set(HMCDCore_src_prefix "${CMAKE_CURRENT_LIST_DIR}/")
set(HMCDCore_inc_prefix "${CMAKE_CURRENT_LIST_DIR}/../../include/HMCDCore")

# Configure version
configure_file("${HMCDCore_inc_prefix}/HMCDCoreVersion.h.in" "${HMCDCore_inc_prefix}/HMCDCoreVersion.h")

# List of files to compile
set(HMCDCore_src_list
                "${HMCDCore_src_prefix}/HMCD.c"
                "${HMCDCore_src_prefix}/DirScanUtil.c"
)

# Build shared and static
add_library(shared_HMCDCore SHARED
                "${HMCDCore_src_list}"
)
add_library(static_HMCDCore STATIC
                "${HMCDCore_src_list}"
)

# Includes
target_include_directories(shared_HMCDCore PUBLIC "${CMAKE_CURRENT_LIST_DIR}" "${HMCDCore_inc_prefix}")
target_include_directories(static_HMCDCore PUBLIC "${CMAKE_CURRENT_LIST_DIR}" "${HMCDCore_inc_prefix}")

# Libraries
target_link_libraries(shared_HMCDCore "curl")
target_link_libraries(static_HMCDCore "curl")

# Common output name
set_target_properties(shared_HMCDCore PROPERTIES OUTPUT_NAME hmcd)
set_target_properties(static_HMCDCore PROPERTIES OUTPUT_NAME hmcd)

# Flags
target_compile_options(shared_HMCDCore PUBLIC "-Wall" "-O2")
target_compile_options(static_HMCDCore PUBLIC "-Wall" "-O2")

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(static_HMCDCore PUBLIC "-Wextra")
    target_compile_options(shared_HMCDCore PUBLIC "-Wextra")
endif()
