#!/bin/bash

RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[0;33m'
BLUE='\033[0;34m'
NC='\033[0m'

header() {
    echo -e "${BLUE}==============================${NC}"
    echo -e "${BLUE} $1 ${NC}"
    echo -e "${BLUE}==============================${NC}"
}

error() {
    echo -e "${RED}Error: $1${NC}"
}

success() {
    echo -e "${GREEN}$1${NC}"
}

header "Initializing and Updating Submodules"
git submodule update --init --recursive
if [ $? -ne 0 ]; then
    error "Failed to initialize and update submodules."
else
    success "Successfully initialized and updated submodules."
fi

header "Setting the CROSS_COMPILE Variable"
export CROSS_COMPILE=arm-linux-gnueabihf-
success "CROSS_COMPILE is set to arm-linux-gnueabihf-"

header "Building the Project"
make all
if [ $? -ne 0 ]; then
    error "Build failed."
else
    success "Build completed successfully."
fi

header "Creating Distribution"
make dist
if [ $? -ne 0 ]; then
    error "Failed to create the distribution."
else
    success "Distribution created successfully."
    header "Build Process Complete"
fi