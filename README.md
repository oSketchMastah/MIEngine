# MIEngine
[![CMake Build and Test](https://github.com/oSketchMastah/MIEngine/actions/workflows/cmake.yml/badge.svg)](https://github.com/oSketchMastah/MIEngine/actions/workflows/cmake.yml)

A collection of types and C++ code that helps.

Works on Linux and Windows. 
Uses concepts from C++20 standard in at least a few things (see MIDetectionTesting.h for an example), otherwise uses many features from C++17.

When I'm feeling clever enough to write something neat, and think it sensible in the eyes of a beholder, you'll find it here.

To run the test in main.cpp yourself:
  1. make sure you have cmake downloaded and installed.

# On Linux
  2. go to MIEngines scripts directory, to the LINUX subdirectory, open the terminal and run the CompileNRun script with "MITest" as the only argument (same argument given to project command in cmake).
# On Windows
  2. Go to MIEngines scripts directory, to the WIN subdirectory, and run CompileNRun.bat.   
      Note: you may need to adjust the path to the cmake executable (to run cmake), and/or vcvarsall.bat (for msbuild to compile the visual studio solution)
  3. If you want to deal with the code in visual studio, go to the bin subdirectory of MIEngine, the .sln should be put there by cmake.
