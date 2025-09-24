cmake_minimum_required(VERSION 3.20)

project(CustomDictionary
  VERSION 0.1.0
  LANGUAGES CXX)

# Require C++20 for modern features and chrono/format
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(GNUInstallDirs)

option(BUILD_TESTING "Build tests" ON)
option(BUILD_BENCHMARKS "Build benchmarks" ON)

add_library(dict_core
  src/dict/IDictionary.hpp
  src/dict/DictionaryConfig.hpp
  src/dict/AVLTreeDictionary.hpp
  src/dict/AVLTreeDictionary.cpp
)
target_include_directories(dict_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_compile_features(dict_core PUBLIC cxx_std_20)

add_executable(dictionary_main src/main.cpp)
target_link_libraries(dictionary_main PRIVATE dict_core)

if(BUILD_BENCHMARKS)
  add_executable(benchmarks src/bench/bench_main.cpp)
  target_link_libraries(benchmarks PRIVATE dict_core)
endif()

if(BUILD_TESTING)
  enable_testing()
  add_executable(unit_tests test/test_smoke.cpp)
  target_link_libraries(unit_tests PRIVATE dict_core)
  add_test(NAME smoke COMMAND unit_tests)
endif()


