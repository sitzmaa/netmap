cmake_minimum_required(VERSION 3.10)

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

# Set the project name
project(netmap_backend)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Define include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/utilities/include
    ${CMAKE_SOURCE_DIR}/externals
)

# Import Boost
set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Boost REQUIRED COMPONENTS system)
include_directories(${Boost_INCLUDE_DIRS})

# Import nlohmann-json
find_package(nlohmann_json REQUIRED)
include_directories(${nlohmann_json_INCLUDE_DIRS})

# Import tinyxml2
find_package(tinyxml2 REQUIRED)

# Find all .cpp files in utilities/src
file(GLOB UTILITIES_SRC_FILES "utilities/src/*.cpp")

# Define libraries based on the .cpp files
foreach(SOURCE_FILE ${UTILITIES_SRC_FILES})
    get_filename_component(LIBRARY_NAME ${SOURCE_FILE} NAME_WE)

    # Create a library for each source file
    add_library(${LIBRARY_NAME} ${SOURCE_FILE})
    
    # Ensure it has access to the include directories
    target_include_directories(${LIBRARY_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/utilities/include)
endforeach()

# Add the server library
add_library(server src/network_server.cpp)
target_link_libraries(server ${Boost_LIBRARIES})

# Create the executable for the main program
add_executable(netmap_backend 
    src/main.cpp 
    src/network_state.cpp 
    src/node.cpp
)

target_link_libraries(netmap_backend
    server
    nlohmann_json::nlohmann_json
    tinyxml2::tinyxml2
)

# Link all dynamically created libraries
foreach(SOURCE_FILE ${UTILITIES_SRC_FILES})
    get_filename_component(LIBRARY_NAME ${SOURCE_FILE} NAME_WE)
    target_link_libraries(netmap_backend ${LIBRARY_NAME})
endforeach()