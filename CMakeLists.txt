cmake_minimum_required(VERSION 3.20)
project(
  calculator
  VERSION 1.0
  LANGUAGES C)

# ------------------------------------------------
# C standard — no compiler extensions
# ------------------------------------------------
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# Export compile_commands.json (clangd / IDEs)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ------------------------------------------------
# Options
# ------------------------------------------------
option(CALCULATOR_BUILD_TESTS "Build calculator tests" ON)
option(CALCULATOR_ENABLE_SANITIZERS "Enable Address Sanitizer in tests" ON)

# ------------------------------------------------
# Portable warning flags
# ------------------------------------------------
if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# ================================================================
# CPM.cmake bootstrap Descarga CPM solo si no existe; usa caché para builds
# offline.
# ================================================================
set(CPM_DOWNLOAD_VERSION 0.40.2)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM.cmake")

if(NOT EXISTS "${CPM_DOWNLOAD_LOCATION}")
  message(
    STATUS "[calculator] Downloading CPM.cmake v${CPM_DOWNLOAD_VERSION}...")
  file(
    DOWNLOAD
    "https://github.com/cpm-cmake/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake"
    "${CPM_DOWNLOAD_LOCATION}"
    STATUS download_status
    TLS_VERIFY ON)
  list(GET download_status 0 status_code)
  list(GET download_status 1 status_msg)
  if(NOT status_code EQUAL 0)
    message(
      FATAL_ERROR "[calculator] Failed to download CPM.cmake: ${status_msg}")
  endif()
endif()

include("${CPM_DOWNLOAD_LOCATION}")

# ================================================================
# Dependencies via CPM
# ================================================================
cpmaddpackage(
  NAME arena GIT_REPOSITORY
  https://laentropia-homelab.tail7368da.ts.net/laentropia/Arena.git GIT_TAG
  main)

cpmaddpackage(
  NAME arraylist GIT_REPOSITORY
  https://laentropia-homelab.tail7368da.ts.net/laentropia/ArrayList.git GIT_TAG
  main)

# ================================================================
# LIBRARY  (lógica reutilizable, separada del ejecutable)
# ================================================================
add_library(calculator_lib src/lexer.c src/parser.c src/evaluator.c)

target_include_directories(
  calculator_lib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                        $<INSTALL_INTERFACE:include>)

target_compile_features(calculator_lib PUBLIC c_std_11)

target_link_libraries(
  calculator_lib
  PUBLIC arena
  PUBLIC arraylist
  PRIVATE m) # libm — solo en el linker interno, no se propaga

# ================================================================
# EXECUTABLE
# ================================================================
add_executable(calculator src/main.c)
target_link_libraries(calculator PRIVATE calculator_lib)

# ================================================================
# TESTS
# ================================================================
if(CALCULATOR_BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()
