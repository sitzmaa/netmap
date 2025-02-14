cmake_minimum_required(VERSION 3.10)
project(NetMapCLI)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Add the source files
set(SOURCES
    main.cpp
    cli.cpp
)

# Add the header files directory
include_directories(${CMAKE_SOURCE_DIR})

# Create the executable
add_executable(netmap_cli ${SOURCES})

# Link with necessary libraries
if(UNIX)
    target_link_libraries(netmap_cli pthread)  # Link pthread for Unix systems
endif()