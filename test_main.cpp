#include "tests/t-index.h"

#include <iostream>

int main(i32 argc, char** argv) {
    // Print compiler
    if constexpr (COMPILER_CLANG == 1)   { std::cout << "[COMPILER] COMPILER_CLANG" << std::endl; }
    if constexpr (COMPILER_GCC == 1)     { std::cout << "[COMPILER] COMPILER_GCC" << std::endl; }
    if constexpr (COMPILER_MSVC == 1)    { std::cout << "[COMPILER] COMPILER_MSVC" << std::endl; }
    if constexpr (COMPILER_UNKNOWN == 1) { std::cout << "[COMPILER] COMPILER_UNKNOWN" << std::endl; }

    // Print OS
    if constexpr (OS_WIN == 1)     { std::cout << "[OS] OS_WIN" << std::endl; }
    if constexpr (OS_LINUX == 1)   { std::cout << "[OS] OS_LINUX" << std::endl; }
    if constexpr (OS_MAC == 1)     { std::cout << "[OS] OS_MAC" << std::endl; }
    if constexpr (OS_UNKNOWN == 1) { std::cout << "[OS] OS_UNKNOWN" << std::endl; }

    if constexpr (UNNAMED_PROJECT_DEBUG == 1) {
        std::cout << "[MODE] DEBUG" << std::endl;
    }
    else {
        std::cout << "[MODE] RELEASE" << std::endl;
    }

    // Print CPU architecture
    std::cout << "[CPU ARCH] " << CPU_ARCH << std::endl;

    i32 exitCode = runAllTests(argc, argv);

    return exitCode;
}
