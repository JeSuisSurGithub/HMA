cmake_minimum_required(VERSION 3.10)

project(HMCD-CLI VERSION 1.3.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Configure version
configure_file("${HMCD-CLI_SRC_DIR}/HMCD-CLI_Version.h.in" "${HMCD-CLI_SRC_DIR}/HMCD-CLI_Version.h")

# Build libraries
include("${HMCD-Core_SRC_DIR}/CMakeLists.txt")

# Source
add_executable(HMCD-CLI
    "${HMCD-CLI_SRC_DIR}/main.c"
)
# Include

target_include_directories(HMCD-CLI PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}"
    "${HMCD-CLI_SRC_DIR}"
    "${HMCD-Core_INC_DIR}"
)
# Link built libraries
target_link_directories(HMCD-CLI PUBLIC "${CMAKE_CURRENT_LIST_DIR}" "${CMAKE_BINARY_DIR}")

# Link with static one
target_link_libraries(HMCD-CLI PUBLIC
    HMCD-Core_static
)

# Flags
if(RELEASE_BUILD EQUAL 1)
    target_compile_options(HMCD-CLI PUBLIC "-Wall" "-O2")
else()
    target_compile_options(HMCD-CLI PUBLIC "-Wall" "-ggdb")
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(HMCD-CLI PUBLIC "-Wextra")
endif()

# Wait first for libraries to build
add_dependencies(HMCD-CLI HMCD-Core_static HMCD-Core_shared)

# Install
install(TARGETS HMCD-CLI
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")