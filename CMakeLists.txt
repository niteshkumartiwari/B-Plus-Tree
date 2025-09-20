cmake_minimum_required(VERSION 3.15)
project(bplus-tree VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler-specific options
if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include directories
include_directories(include)

# Create library
add_library(bptree STATIC
    src/display.cpp
    src/insertion.cpp
    src/removal.cpp
    src/search.cpp
    src/utils.cpp
)

target_include_directories(bptree PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Main executable
add_executable(bptree_demo src/main.cpp)
target_link_libraries(bptree_demo bptree)

# Create DBFiles directory
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/DBFiles)

# Copy test script
configure_file(tests/test_suite.sh ${CMAKE_CURRENT_BINARY_DIR}/test_suite.sh COPYONLY)

# Enable testing
enable_testing()

# Add custom test target
add_test(NAME comprehensive_tests 
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_suite.sh
         WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Installation
install(TARGETS bptree bptree_demo
        EXPORT bptree-targets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin)

install(DIRECTORY include/ DESTINATION include)

# Export configuration
install(EXPORT bptree-targets
        FILE bptree-config.cmake
        DESTINATION lib/cmake/bptree)