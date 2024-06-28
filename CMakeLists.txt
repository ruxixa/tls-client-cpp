#
# This file is a part of tls-client implementation for
# modern C++ (17+ standard)
#
# Thanks for bogdanfinn for creating the original tls-client
# library in GO https://github.com/bogdanfinn/tls-client
# 
cmake_minimum_required(VERSION 3.8)

project(tls-client-cpp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  if(CMAKE_C_COMPILER_ID STREQUAL "MSVC" AND CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue,ProgramDatabase>")
  endif()
endif()

option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" OFF)

if(BUILD_TESTS)
  enable_testing()
  add_subdirectory(tests)

  set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/dependencies")
  set(DEST_DIR "${CMAKE_BINARY_DIR}/tests/dependencies")

  add_custom_target(copy_dependencies ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${SOURCE_DIR} ${DEST_DIR}
  )
endif()

if(BUILD_EXAMPLES)
    # TODO!!!
    # add_subdirectory(examples)
endif()

