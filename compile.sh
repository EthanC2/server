#!/bin/bash

# Variables
COMPILER=g++
VERSION='-std=c++2a'
WARNINGS='-Wextra -Wall'

# Compile code
$COMPILER $VERSION $WARNINGS ./src/*.cpp -o ./build/server