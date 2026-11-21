cmake_minimum_required(VERSION 3.14)
project(Lab1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_COVERAGE "Instrument the code with gcov to measure test coverage" OFF)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
  if(ENABLE_COVERAGE)
    add_compile_options(--coverage -O0 -g)
    add_link_options(--coverage)
  endif()
elseif(MSVC)
  add_compile_options(/W4)
endif()

# ---- GoogleTest: use the installed one, otherwise download it ----
if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()
find_package(GTest QUIET)
if(NOT GTest_FOUND)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
endif()
if(TARGET GTest::gtest_main)
  set(GTEST_MAIN_TARGET GTest::gtest_main)
else()
  set(GTEST_MAIN_TARGET gtest_main)
endif()

enable_testing()
include(GoogleTest)

add_subdirectory(rectangle)
add_subdirectory(set)

# ---- HTML test report:  cmake --build build --target test_report ----
find_package(Python3 COMPONENTS Interpreter)
if(Python3_FOUND)
  add_custom_target(
    test_report
    COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/tools/gtest_to_html.py
            --output ${CMAKE_BINARY_DIR}/test_report.html
            $<TARGET_FILE:TestRectangle> $<TARGET_FILE:TestSet>
    DEPENDS TestRectangle TestSet
    COMMENT "Running the tests and writing test_report.html"
    VERBATIM)
else()
  message(STATUS "Python 3 not found: the test_report target is not available")
endif()

# ---- Coverage report (needs -DENABLE_COVERAGE=ON and gcovr) ----
if(ENABLE_COVERAGE)
  find_program(GCOVR_EXECUTABLE gcovr)
  if(GCOVR_EXECUTABLE)
    add_custom_target(
      coverage
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
      COMMAND $<TARGET_FILE:TestRectangle>
      COMMAND $<TARGET_FILE:TestSet>
      COMMAND ${GCOVR_EXECUTABLE} --root ${CMAKE_SOURCE_DIR}
              --filter ".*/Rectangle\\.cpp" --filter ".*/Set\\.cpp"
              --html-details ${CMAKE_BINARY_DIR}/coverage/index.html
              --print-summary --fail-under-line 90
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
      DEPENDS TestRectangle TestSet
      COMMENT "Running the tests and measuring coverage"
      VERBATIM)
  else()
    message(STATUS "gcovr not found: the coverage target is not available")
  endif()
endif()

# ---- Documentation:  cmake --build build --target docs ----
find_package(Doxygen)
if(DOXYGEN_FOUND)
  set(DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
  set(DOXYGEN_EXTRACT_ALL YES)
  set(DOXYGEN_GENERATE_HTML YES)
  doxygen_add_docs(
    docs
    ${CMAKE_SOURCE_DIR}/rectangle/src/Rectangle.h ${CMAKE_SOURCE_DIR}/rectangle/src/Rectangle.cpp
    ${CMAKE_SOURCE_DIR}/set/src/Set.h ${CMAKE_SOURCE_DIR}/set/src/Set.cpp
    COMMENT "Generating Doxygen documentation")
endif()