# ================================================================
# FIND CMOCKA  (system → FetchContent as fallback)
# ================================================================
find_package(cmocka QUIET)

if(NOT cmocka_FOUND)
  message(
    STATUS
      "[calculator] cmocka not found on system — fetching with FetchContent...")

  include(FetchContent)
  FetchContent_Declare(
    cmocka
    GIT_REPOSITORY https://git.cryptomilk.org/projects/cmocka.git
    GIT_TAG cmocka-1.1.7
    GIT_SHALLOW TRUE)

  # Static lib only, no cmocka examples or self-tests
  set(WITH_STATIC_LIB
      ON
      CACHE BOOL "" FORCE)
  set(WITH_SHARED_LIB
      OFF
      CACHE BOOL "" FORCE)
  set(WITH_CMOCKERY_SUPPORT
      OFF
      CACHE BOOL "" FORCE)
  set(WITH_EXAMPLES
      OFF
      CACHE BOOL "" FORCE)
  set(UNIT_TESTING
      OFF
      CACHE BOOL "" FORCE)
  set(PICKY_DEVELOPER
      OFF
      CACHE BOOL "" FORCE)

  FetchContent_MakeAvailable(cmocka)

  # Normalize the target name (varies across cmocka versions)
  if(NOT TARGET cmocka::cmocka)
    if(TARGET cmocka-static)
      add_library(cmocka::cmocka ALIAS cmocka-static)
    elseif(TARGET cmocka)
      add_library(cmocka::cmocka ALIAS cmocka)
    else()
      message(
        WARNING "[calculator] Could not create cmocka::cmocka — skipping tests."
      )
      return()
    endif()
  endif()
endif()

# ================================================================
# DETECT ADDRESS SANITIZER SUPPORT
# ================================================================
set(CALCULATOR_USE_ASAN OFF)

if(CALCULATOR_ENABLE_SANITIZERS)
  if(MSVC)
    if(MSVC_VERSION GREATER_EQUAL 1928)
      set(CALCULATOR_USE_ASAN ON)
    else()
      message(STATUS "[calculator] MSVC < 16.9: ASAN not available.")
    endif()
  elseif(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang" AND NOT WIN32)
    include(CheckCCompilerFlag)
    check_c_compiler_flag(-fsanitize=address CALCULATOR_COMPILER_HAS_ASAN)
    if(CALCULATOR_COMPILER_HAS_ASAN)
      set(CALCULATOR_USE_ASAN ON)
    else()
      message(
        STATUS "[calculator] Compiler does not support -fsanitize=address.")
    endif()
  else()
    message(STATUS "[calculator] Unknown platform/compiler — ASAN skipped.")
  endif()

  if(CALCULATOR_USE_ASAN)
    message(STATUS "[calculator] Address Sanitizer enabled.")
  endif()
endif()

# ================================================================
# Helper: apply common settings to every test target
# ================================================================
function(calculator_add_test target source)
  add_executable(${target} ${source})

  target_link_libraries(${target} PRIVATE calculator_lib cmocka::cmocka)

  target_compile_features(${target} PRIVATE c_std_11)

  if(CALCULATOR_USE_ASAN)
    if(MSVC)
      target_compile_options(${target} PRIVATE /fsanitize=address)
    else()
      target_compile_options(${target} PRIVATE -fsanitize=address
                                               -fno-omit-frame-pointer)
      target_link_options(${target} PRIVATE -fsanitize=address)
    endif()
  endif()

  add_test(NAME ${target} COMMAND ${target})
  set_tests_properties(${target} PROPERTIES TIMEOUT 30)
endfunction()

# ================================================================
# TEST TARGETS
# ================================================================
calculator_add_test(test_lexer test_lexer.c)
calculator_add_test(test_parser test_parser.c)
calculator_add_test(test_evaluator test_evaluator.c)
