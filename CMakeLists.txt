cmake_minimum_required(VERSION 3.30)

set(CMAKE_CXX_STANDARD 20 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(
  Snowglobe
  VERSION 1.0
  LANGUAGES CXX)

add_subdirectory(src)
add_subdirectory(thirdparty)