cmake_minimum_required(VERSION 3.30)

project(yaml-parser VERSION 1.0 DESCRIPTION "C++ YAML parser" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*)

add_definitions(-DCMAKE_EXPORT_COMPILE_COMMANDS=ON)


find_package(GTest REQUIRED)

add_library(parser-lib SHARED
    src/tokenizer.cc
)


add_executable(parser-test
    test/test_tokenizer.cc)

target_include_directories(parser-test
    PUBLIC src)

target_link_libraries(parser-test
    parser-lib
    GTest::gtest_main
)

file(GLOB_RECURSE source_files LIST_DIRECTORIES false *.cc *.h)

add_custom_target(format
    COMMAND clang-format --style=file -i ${source_files}
)
