cmake_minimum_required(VERSION 3.20)
project(lib_asmkraken)

set(CMAKE_CXX_STANDARD 20)

string(REPLACE "-" ";" TRIPLET_LIST ${VCPKG_TARGET_TRIPLET})
list(GET TRIPLET_LIST 0 LIBASMKRAKEN_PLATFORM_TARGET)
list(GET TRIPLET_LIST 1 LIBASMKRAKEN_OS_TARGET)

add_definitions( -DASMKRAKEN_${LIBASMKRAKEN_PLATFORM_TARGET} )

find_path(YARA_INCLUDE_DIR yara.h)
find_library(YARA_LIBRARY libyara)

add_library(lib_asmkraken include/asmkraken.h src/mem/Pointer.cpp include/mem/Pointer.h include/asm/asm.h src/asm/asm.cpp src/mem/scanning/PatternScanner.cpp include/mem/scanning/PatternScanner.h src/mem/scanning/PatternScanMatch.cpp include/mem/scanning/PatternScanMatch.h src/mem/scanning/PatternScanMeta.cpp include/mem/scanning/PatternScanMeta.h src/mem/scanning/PatternScanRule.cpp include/mem/scanning/PatternScanRule.h src/mem/scanning/PatternScanContext.cpp include/mem/scanning/PatternScanContext.h include/asm/Patch.h src/asm/Patch.cpp include/asm/MidFunctionHook.h src/asm/MidFunctionHook.cpp include/mem/mem.h src/mem/mem.cpp include/Toggleable.h)

target_include_directories(lib_asmkraken PRIVATE ${YARA_INCLUDE_DIR})
target_link_libraries(lib_asmkraken ${YARA_LIBRARY})

install(
        DIRECTORY "include/"
        DESTINATION "include/asmkraken"
)
install(TARGETS lib_asmkraken DESTINATION "${main_lib_dest}")