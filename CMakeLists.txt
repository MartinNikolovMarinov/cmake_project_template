cmake_minimum_required(VERSION 3.2)

set(executable_name unnamed_project)
string(TOUPPER ${executable_name} executable_name_uppercase)
set(main_file main.cpp)
set(cpp_version 20)

project(${executable_name} VERSION 0.0.1)

# Standard requirenments:

set(CMAKE_CXX_STANDARD ${cpp_version})
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Generates compile_commands.json

# Force cmake to put all libraries and executables in the same directory, so that special compilers can find them:

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Download and update git submodules:

function(update_git_submodules)
    find_package(Git QUIET)
    if (GIT_FOUND)
        message(STATUS "[${executable_name_uppercase}] Submodule update")
        execute_process(
            COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            RESULT_VARIABLE GIT_SUBMOD_RESULT
        )
        if (NOT GIT_SUBMOD_RESULT EQUAL "0")
            message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
        endif()
    else()
        message(FATAL_ERROR "Git is not found. It is required to update submodules.")
    endif()
endfunction()

update_git_submodules()

# Includes:

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    "${CMAKE_SOURCE_DIR}/cmake"
)

include(DetectOS)
include(CodeCoverage)
include(CompilerOptions)

# ---------------------------------------- Begin Options ---------------------------------------------------------------

if(NOT CMAKE_BUILD_TYPE OR CMAKE_BUILD_TYPE STREQUAL "Release")
    set(${executable_name_uppercase}_DEBUG OFF)
else()
    set(${executable_name_uppercase}_DEBUG ON)
endif()

option(${executable_name_uppercase}_BUILD_TESTS "Build tests." OFF)
option(${executable_name_uppercase}_TEST_COVERAGE "Enable code coverage for tests." OFF)

message(STATUS "[${executable_name_uppercase}] ---------------------------------------------")
message(STATUS "[${executable_name_uppercase}] Version:       ${PROJECT_VERSION}")
message(STATUS "[${executable_name_uppercase}] Platform:      ${OS}")
message(STATUS "[${executable_name_uppercase}] Arch:          ${CMAKE_HOST_SYSTEM_PROCESSOR}")
message(STATUS "[${executable_name_uppercase}] C++ Version:   ${CMAKE_CXX_STANDARD}")
message(STATUS "[${executable_name_uppercase}] Compiler:      ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "[${executable_name_uppercase}] Debug:         ${${executable_name_uppercase}_DEBUG}")
message(STATUS "[${executable_name_uppercase}] Build Tests:   ${${executable_name_uppercase}_BUILD_TESTS}")
message(STATUS "[${executable_name_uppercase}] Code Coverage: ${${executable_name_uppercase}_TEST_COVERAGE}")

# ---------------------------------------- End Options -----------------------------------------------------------------

# Default compiler options macro:

macro(target_set_default_flags target)

    if(OS STREQUAL "linux")
        set(common_flags "-std=c++${cpp_version}")
        set(debug_flags "")
        set(release_flags "")
    elseif(OS STREQUAL "darwin")
        set(common_flags "-std=c++${cpp_version}")
        set(debug_flags "")
        set(release_flags "")
    elseif(OS STREQUAL "windows")
        set(common_flags "/std:c++${cpp_version}")
        set(debug_flags "")
        set(release_flags "")
    endif()

    generate_common_flags(
        common_flags "${common_flags}"
        debug_flags "${debug_flags}"
        release_flags "${release_flags}"
    )

    if(${executable_name_uppercase}_DEBUG)
        target_compile_options(${target} PRIVATE ${common_flags} ${debug_flags})
    else()
        target_compile_options(${target} PRIVATE ${common_flags} ${release_flags})
    endif()

endmacro()

# ---------------------------------------- Begin Create Executable -----------------------------------------------------

add_subdirectory(lib/core)

set(src_files

   src/init_core.cpp
)

add_executable(${executable_name} ${main_file} ${src_files})

target_include_directories(${executable_name} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(${executable_name} PUBLIC
    ${executable_name_uppercase}_DEBUG="$<BOOL:${executable_name_uppercase}_DEBUG>"
    ${executable_name_uppercase}_BINARY_PATH="${CMAKE_BINARY_DIR}/"
)

target_link_libraries(${executable_name} PUBLIC
    core
)

target_set_default_flags(${executable_name})

# ---------------------------------------- End Create Executable -----------------------------------------------------

# if (${executable_name_uppercase}_BUILD_TESTS)

#     # ---------------------------------------- Begin Create Test Executable --------------------------------------------

#     set(test_files
#         tests/t-index.cpp
#     )

#     add_executable(${executable_name}_test test_${main_file} ${test_files} ${src_files})

#     target_include_directories(${executable_name}_test PUBLIC
#         ${CMAKE_CURRENT_SOURCE_DIR}/include
#     )

#     target_compile_definitions(${executable_name}_test PUBLIC
#         ${executable_name_uppercase}="$<BOOL:${executable_name_uppercase}_DEBUG>"
#         ${executable_name_uppercase}_BINARY_PATH="${CMAKE_BINARY_DIR}/"
#     )

#     target_link_libraries(${executable_name}_test PUBLIC
#         core
#     )

#     target_set_default_flags(${executable_name}_test)

#     if(CORE_CODE_COVERAGE)
#         if (CMAKE_COMPILER_IS_GNUCXX)
#             include(CodeCoverage)
#             append_coverage_compiler_flags_to_target(${test_executable})
#             append_coverage_compiler_flags_to_target(${executable_name})
#             setup_target_for_coverage_lcov(
#                 NAME ${test_executable}_coverage
#                 EXECUTABLE ${test_executable}
#                 DEPENDENCIES ${test_executable}
#             )
#         else()
#             message(WARNING "[CORE] Code coverage is not supported for ${CMAKE_CXX_COMPILER_ID} compiler.")
#         endif()
#     endif()

#     # Add the test
#     enable_testing()
#     add_test(NAME ${executable_name}_test COMMAND ${executable_name}_test)

#     # ---------------------------------------- End Create Test Executable ----------------------------------------------
# endif()

message(STATUS "[${executable_name_uppercase}] ---------------------------------------------")
