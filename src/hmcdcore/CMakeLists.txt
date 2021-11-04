cmake_minimum_required(VERSION 3.10)

project(HMCD-Core VERSION 2.2.0)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# Configure version and debugging
configure_file("${HMCD-Core_INC_DIR}/hmcdcore_version.h.in" "${HMCD-Core_INC_DIR}/hmcdcore_version.h")

# List of files to compile
set(HMCD-Core_SRC_LIST
    "${HMCD-Core_SRC_DIR}/dir_scan_util.c"
    "${HMCD-Core_SRC_DIR}/hmcd.c")

# Build shared and static
add_library(HMCD-Core_static STATIC "${HMCD-Core_SRC_LIST}")
add_library(HMCD-Core_shared SHARED "${HMCD-Core_SRC_LIST}")

# Includes
target_include_directories(HMCD-Core_static PUBLIC "${CMAKE_CURRENT_LIST_DIR}" "${HMCD_INC_DIR}/")
target_include_directories(HMCD-Core_shared PUBLIC "${CMAKE_CURRENT_LIST_DIR}" "${HMCD_INC_DIR}/")

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
target_compile_options(HMCD-Core_static PUBLIC "${HMCD_FLAGS}")
target_compile_options(HMCD-Core_shared PUBLIC "${HMCD_FLAGS}")


# Install
install(TARGETS HMCD-Core_static HMCD-Core_shared
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib")

install(FILES
    "${HMCD-Core_INC_DIR}/dir_scan_util.h"
    "${HMCD-Core_INC_DIR}/hmcd.h"
    "${HMCD-Core_INC_DIR}/hmcdcore_version.h"
    "${HMCD-Core_INC_DIR}/hmcppd.hpp"
    DESTINATION "${CMAKE_INSTALL_PREFIX}/include/hmcdcore")