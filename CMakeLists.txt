cmake_minimum_required(VERSION 3.20)
project(lib_asmkraken)

set(CMAKE_CXX_STANDARD 20)

add_library(lib_asmkraken src/mem/Pointer.cpp include/mem/Pointer.h include/asm/asm.h src/asm/asm.cpp src/mem/scanning/PatternScanner.cpp include/mem/scanning/PatternScanner.h src/mem/scanning/PatternScanMatch.cpp include/mem/scanning/PatternScanMatch.h src/mem/scanning/PatternScanMeta.cpp include/mem/scanning/PatternScanMeta.h src/mem/scanning/PatternScanRule.cpp include/mem/scanning/PatternScanRule.h src/mem/scanning/PatternScanContext.cpp include/mem/scanning/PatternScanContext.h)

install(
        DIRECTORY "include/"
        DESTINATION "include"
)
install(TARGETS lib_asmkraken DESTINATION "${main_lib_dest}")