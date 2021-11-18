cmake_minimum_required(VERSION 3.20)
project(lib_asmkraken)

set(CMAKE_CXX_STANDARD 20)

add_library(lib_asmkraken src/mem/Pointer.cpp src/mem/Pointer.h src/asm/asm.h src/asm/asm.cpp src/mem/scanning/PatternScanner.cpp src/mem/scanning/PatternScanner.h src/mem/scanning/PatternScanMatch.cpp src/mem/scanning/PatternScanMatch.h src/mem/scanning/PatternScanMeta.cpp src/mem/scanning/PatternScanMeta.h src/mem/scanning/PatternScanRule.cpp src/mem/scanning/PatternScanRule.h src/mem/scanning/PatternScanContext.cpp src/mem/scanning/PatternScanContext.h)
