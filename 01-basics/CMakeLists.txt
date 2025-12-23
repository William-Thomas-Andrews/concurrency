cmake_minimum_required(VERSION 3.22.1)
project(HashTableProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compile warnings
if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Library target
add_library(hashtable_lib
    src/HashTable.cpp
)
target_include_directories(hashtable_lib PUBLIC include)

# Main executable
add_executable(main src/Main.cpp)
target_link_libraries(main hashtable_lib)

# Enable testing
enable_testing()
add_subdirectory(test)