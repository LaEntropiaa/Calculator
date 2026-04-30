cmake_minimum_required(VERSION 3.20)
project(calculator VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
)

include(cmake/CPM.cmake)

CPMAddPackage(
    NAME arena
    GIT_REPOSITORY https://laentropia-homelab.tail7368da.ts.net/laentropia/Arena.git
    GIT_TAG main
)

CPMAddPackage(
    NAME arraylist
    GIT_REPOSITORY https://laentropia-homelab.tail7368da.ts.net/laentropia/ArrayList.git
    GIT_TAG main
)

add_library(calculator_lib
    src/lexer.c
    src/parser.c
    src/evaluator.c
)

target_include_directories(calculator_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(calculator_lib
    PUBLIC arena
    PUBLIC arraylist
)

add_executable(calculator src/main.c)
target_link_libraries(calculator calculator_lib)

enable_testing()
add_subdirectory(test)
