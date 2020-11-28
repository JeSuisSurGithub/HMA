cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(HMCD VERSION 1.0)

# Source files
add_executable(HMCD 
                "main.c" 
                "HMCD.c" 
                "directScanUtil.c" 
)

# Flags
target_compile_options(HMCD PUBLIC "-Wall" "-O2")

# Includes
target_include_directories(HMCD PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${LIBCURL_DIR}/include"
)

# Libraries
# if Windows
if (WIN32)
link_directories("${LIBCURL_DIR}/lib")
target_link_libraries(HMCD 
			"${LIBCURL_DIR}/lib/libcurl.dll.a"
			"${LIBCURL_DIR}/lib/libcurl.a"
)
# If not
else()
target_link_libraries(HMCD
			"libcurl.so"
)
endif()

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)