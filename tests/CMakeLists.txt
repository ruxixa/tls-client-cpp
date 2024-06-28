#
# This file is a part of tls-client implementation for
# modern C++ (17+ standard)
#
# Thanks for bogdanfinn for creating the original tls-client
# library in GO https://github.com/bogdanfinn/tls-client
# 
cmake_minimum_required(VERSION 3.14)

project(tls-client-cpp-tests)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  tls-client-cpp-tests
  TlsClientTest.cpp
)

target_link_libraries(
  tls-client-cpp-tests
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(tls-client-cpp-tests)