cmake_minimum_required(VERSION 3.20)
project(calculator VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Export compile_commands.json (para clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
)


include_directories(include)

add_library(calculator_lib
    src/lexer.c
    src/parser.c
    src/ast.c
    src/evaluator.c
)

add_executable(calculator src/main.c)

target_link_libraries(calculator calculator_lib)

# ------------------------
# Testing
# ------------------------

enable_testing()
add_subdirectory(test)

