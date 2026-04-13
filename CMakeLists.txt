cmake_minimum_required(VERSION 3.20)
project(calculator VERSION 1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
include(FetchContent)

FetchContent_Declare(
    arena
    GIT_REPOSITORY https://laentropia-homelab.tail7368da.ts.net/laentropia/Arena.git
    GIT_TAG main
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/arena
)

# Export compile_commands.json (para clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(
    -Wall
    -Wextra
    -Wpedantic
)


FetchContent_MakeAvailable(arena)

add_library(arena STATIC
    external/arena/src/arena.c
)

target_include_directories(arena
    PUBLIC ${CMAKE_SOURCE_DIR}/external/arena/include
)

add_library(calculator_lib
    src/lexer.c
    src/parser.c
    src/evaluator.c
    src/ASTNodeArray.c
)

target_include_directories(calculator_lib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(calculator_lib
    PUBLIC arena
)

add_executable(calculator src/main.c)

target_link_libraries(calculator calculator_lib)

# ------------------------
# Testing
# ------------------------

enable_testing()
add_subdirectory(test)

