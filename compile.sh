#!/bin/bash

# Compile code with provided main function and appropriate gcc options
# Give full/relative path name to main function (e.g. ./src/test/MTPushTest.cpp)
if [ -z $1 ]
then
mainProgram="main.cpp"
else
mainProgram=$1
fi

echo "compiling code with $mainProgram"

g++ -std=c++11 -pthread -g -Isrc/include $mainProgram
