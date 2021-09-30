# Minimum
cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(HMCD-CLI VERSION 1.1.1)

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Define paths
set(HMCD-CLI_src_prefix "${CMAKE_CURRENT_LIST_DIR}")

# Configure version
configure_file("${HMCD-CLI_src_prefix}/HMCD-CLI_Version.h.in" "${HMCD-CLI_src_prefix}/HMCD-CLI_Version.h")

# Build libraries
include("${CMAKE_CURRENT_LIST_DIR}/../HMCD/CMakeLists.txt")

# Link icon if windows
add_executable(HMCD-CLI
                "${HMCD-CLI_src_prefix}/main.c"
)

# Includes
target_include_directories(HMCD-CLI PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}"
    "${HMCD-CLI_src_prefix}"
    "${CMAKE_CURRENT_LIST_DIR}/../../include"
)

# Link built libraries
target_link_directories(HMCD-CLI PUBLIC "${CMAKE_CURRENT_LIST_DIR}" "${CMAKE_BINARY_DIR}")

# Link with static one
target_link_libraries(HMCD-CLI PUBLIC
                        static_HMCD
)

# Flags
target_compile_options(HMCD-CLI PUBLIC "-Wall" "-O2")
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(HMCD-CLI PUBLIC "-Wextra")
endif()

# Wait first for libraries to build
add_dependencies(HMCD-CLI shared_HMCD static_HMCD)
