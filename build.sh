#!/bin/bash

# Function to display usage instructions
usage() {
    echo "Usage: $0 [-c] [-s]"
    echo "Options:"
    echo "  -c  Build the CLI frontend (located at ./frontend/netmap_cli/build)"
    echo "  -s  Build the backend program (located at ./backend/build)"
    echo "  --help  Display this help message"
    exit 1
}

# Parse command-line arguments
while getopts "cs" opt; do
    case ${opt} in
        c)
            BUILD_CLI=true
            ;;
        s)
            BUILD_BACKEND=true
            ;;
        *)
            usage
            ;;
    esac
done

# Build the CLI
if [ "$BUILD_CLI" = true ]; then
    echo "Building the CLI frontend..."
    cd ./frontend/netmap_cli
    mkdir -p build
    cd build
    make
    if [ $? -eq 0 ]; then
        echo "CLI frontend built successfully!"
    else
        echo "Error building the CLI frontend!"
        exit 1
    fi
    cd ../../..
fi

# Build the backend
if [ "$BUILD_BACKEND" = true ]; then
    echo "Building the backend program..."
    cd ./backend
    mkdir -p build
    cd build
    make
    if [ $? -eq 0 ]; then
        echo "Backend program built successfully!"
    else
        echo "Error building the backend program!"
        exit 1
    fi
    cd ../../..
fi

# If neither -c nor -s is provided, show usage
if [ "$BUILD_CLI" != true ] && [ "$BUILD_BACKEND" != true ]; then
    usage
fi