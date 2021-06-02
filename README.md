# MIEngine
[![CMake Build and Test](https://github.com/oSketchMastah/MIEngine/actions/workflows/cmake.yml/badge.svg)](https://github.com/oSketchMastah/MIEngine/actions/workflows/cmake.yml)

A collection of types and C++ code that helps.

Everything is tested to work on Linux and Windows.

Uses concepts from C++20 standard in at least a few things (see MIDetectionTesting.h for an example), otherwise many features from C++17 are used through-out.

To run the tests in the tests directory yourself:
  1. make sure you have cmake downloaded and installed.

# On Linux
  2. go to MIEngines scripts directory, to the LINUX subdirectory, and run the CompileNTest script
# On Windows
  2. Go to MIEngines scripts directory, to the WIN subdirectory, and run CompileNTest.bat.
      Note: you may need to adjust the path to the cmake executable (to run cmake), and/or vcvarsall.bat (for msbuild to compile the visual studio solution)
  3. If you want to deal with the code in visual studio, go to the build subdirectory created in MIEngine, the .sln should be put there by cmake.
