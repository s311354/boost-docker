#!/bin/bash

for fld in $(find . -name "CMakeLists.txt" -exec dirname {} \;)

do
    for cmakefile in CMakeCache.txt  CMakeFiles cmake_install.cmake Makefile
    do
        rm -rfv $fld/$cmakefile
    done

    # remove executable files
    rm -rfv ./build
done
