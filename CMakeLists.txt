# cli commands
# mkdir build && cd build
# cmkake .. -> create build directory
# make -> build al targets
# ./test_lexer run individual tests
# make test -> run all tests
# make clean



# Specify the minimum version of CMake
cmake_minimum_required(VERSION 3.28.3)

# Project name and settings
project(Chimp LANGUAGES C)

# Enable export of compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set the C standard to C2x
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add compile flags (equivalent to CFLAGS in Makefile)
add_compile_options(-Wall -Wextra -g -pedantic-errors -Wconversion)

# Define source directories
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(TESTS_DIR ${CMAKE_SOURCE_DIR}/tests)

# Main program source files
set(SOURCES
    ${SRC_DIR}/main.c
    ${SRC_DIR}/lexer.c
    ${SRC_DIR}/parser.c
    ${SRC_DIR}/ast.c
    ${SRC_DIR}/value.c
    ${SRC_DIR}/printer.c
)

# Include directories for all targets
include_directories(${SRC_DIR})
include_directories(${TESTS_DIR})

# Main executable
add_executable(chimp ${SOURCES})

# Lexer Test
set(TEST_LEXER_SOURCES
    ${TESTS_DIR}/test_lexer.c
    ${TESTS_DIR}/test_helper_functions.c
    ${SRC_DIR}/lexer.c
)
add_executable(test_lexer ${TEST_LEXER_SOURCES})

# Parser Test
set(TEST_PARSER_SOURCES
    ${TESTS_DIR}/test_parser.c
    ${TESTS_DIR}/test_helper_functions.c
    ${TESTS_DIR}/write_output_to_buffer.c
    ${SRC_DIR}/lexer.c
    ${SRC_DIR}/parser.c
    ${SRC_DIR}/ast.c
    ${SRC_DIR}/value.c
)
add_executable(test_parser ${TEST_PARSER_SOURCES})

# Value Test
set(TEST_VALUE_SOURCES
    ${TESTS_DIR}/test_value.c
    ${SRC_DIR}/value.c
)
add_executable(test_value ${TEST_VALUE_SOURCES})

# Test Runner Target (Runs all tests sequentially)
add_custom_target(
    test
    COMMAND $<TARGET_FILE:test_lexer>
    COMMAND $<TARGET_FILE:test_parser>
    COMMAND $<TARGET_FILE:test_value>
    DEPENDS test_lexer test_parser test_value
)

# Clean Target (Remove executables)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES
    "${CMAKE_BINARY_DIR}/chimp;${CMAKE_BINARY_DIR}/test_lexer;${CMAKE_BINARY_DIR}/test_parser;${CMAKE_BINARY_DIR}/test_value"
)

