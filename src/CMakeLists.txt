cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(HMCD VERSION 1.0)

# Source files
add_executable(HMCD 
                "${CMAKE_CURRENT_LIST_DIR}/main.c" 
                "${CMAKE_CURRENT_LIST_DIR}/HMCD.c" 
                "${CMAKE_CURRENT_LIST_DIR}/directScanUtil.c" 
)

# Includes
target_include_directories(HMCD PUBLIC
                          "${CMAKE_CURRENT_LIST_DIR}"
)

# Libraries
target_link_libraries(HMCD 
			"curl"
)

# Flags
target_compile_options(HMCD PUBLIC "-Wall" "-O2")

# Specify the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)