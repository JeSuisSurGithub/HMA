cmake_minimum_required(VERSION 3.10)

project(HMCD-CLI VERSION 2.0.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Configure version
configure_file("${HMCD-CLI_SRC_DIR}/hmcd-cli_version.h.in" "${HMCD-CLI_SRC_DIR}/hmcd-cli_version.h")

# Build libraries
include("${HMCD-Core_SRC_DIR}/CMakeLists.txt")

# Source
add_executable(HMCD-CLI "${HMCD-CLI_SRC_DIR}/main.c")

# Include
target_include_directories(HMCD-CLI PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}"
    "${HMCD-CLI_SRC_DIR}"
    "${HMCD-Core_INC_DIR}"
)

# Lower case name
set_target_properties(HMCD-CLI PROPERTIES OUTPUT_NAME hmcd-cli)

# Link built libraries
target_link_directories(HMCD-CLI PUBLIC "${CMAKE_CURRENT_LIST_DIR}" "${CMAKE_BINARY_DIR}")

# Link with static one
target_link_libraries(HMCD-CLI PUBLIC
    HMCD-Core_static
)

# Flags
target_compile_options(HMCD-CLI PUBLIC "${HMCD_FLAGS}")

# Wait first for libraries to build
add_dependencies(HMCD-CLI HMCD-Core_static HMCD-Core_shared)

# Install
install(TARGETS HMCD-CLI
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")