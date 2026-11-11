cmake_minimum_required(VERSION 3.16)
project(fundamentos CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.18.0
  GIT_SHALLOW    TRUE
)
FetchContent_MakeAvailable(googletest)

# Código de produção
add_library(separar separar.cpp)
target_include_directories(separar PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_options(separar PRIVATE -Wall -Wextra -Wpedantic)

# Testes
enable_testing()

add_executable(separar_test separar_test.cpp)
target_link_libraries(separar_test PRIVATE separar GTest::gtest_main)
target_compile_options(separar_test PRIVATE -Wall -Wextra -Wpedantic)

include(GoogleTest)
gtest_discover_tests(separar_test)
