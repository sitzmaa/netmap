cmake_minimum_required(VERSION 3.10)

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

# Set the project name
project(netmap_backend)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the include directories for libraries
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/externals)

# Importing Boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS system)

include_directories(${Boost_INCLUDE_DIRS})

# Import nlohmann-json
find_package(nlohmann_json REQUIRED)
include_directories(${nlohmann_json_INCLUDE_DIRS})

# Add the libraries
add_library(traceroute src/utilities/traceroute.cpp)
add_library(ping src/utilities/ping.cpp)
add_library(dns_lookup src/utilities/dns_lookup.cpp)
add_library(network_scan src/utilities/network_scan.cpp)
add_library(server src/network_server.cpp)
target_link_libraries(server ${Boost_LIBRARIES})

# Create an executable for the main program that links with the server library
add_executable(netmap_backend src/main.cpp src/network_state.cpp src/node.cpp)

# Link the server library to the executable
target_link_libraries(netmap_backend
    server
    traceroute
    ping
    dns_lookup
    network_scan
    nlohmann_json::nlohmann_json
)
