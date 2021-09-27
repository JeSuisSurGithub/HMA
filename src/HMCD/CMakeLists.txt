# Minimum
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(HMCD VERSION 1.1.1)

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Define paths
set(HMCD_src_prefix "${CMAKE_CURRENT_LIST_DIR}")

# Configure version
configure_file("${HMCD_src_prefix}/HMCDVersion.h.in" "${HMCD_src_prefix}/HMCDVersion.h")

# Build libraries
include("${CMAKE_CURRENT_LIST_DIR}/../HMCDCore/CMakeLists.txt")

# Link icon if windows
add_executable(HMCD
                "${HMCD_src_prefix}/main.c"
)

# Includes
target_include_directories(HMCD PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}"
    "${HMCD_src_prefix}"
    "${CMAKE_CURRENT_LIST_DIR}/../../include/HMCDCore"
)

# Link built libraries
target_link_directories(HMCD PUBLIC "${CMAKE_CURRENT_LIST_DIR}" "${CMAKE_BINARY_DIR}")

# Link with static one
target_link_libraries(HMCD PUBLIC
                        static_HMCDCore
)

# Flags
target_compile_options(HMCD PUBLIC "-Wall" "-O2")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(HMCD PUBLIC "-Wextra")
endif()

# Wait first for libraries to build
add_dependencies(HMCD shared_HMCDCore static_HMCDCore)
