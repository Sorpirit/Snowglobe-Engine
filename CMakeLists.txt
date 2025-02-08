cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(
  Snowglobe
  VERSION 1.0
  LANGUAGES CXX C)

add_subdirectory(thirdparty)

#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)

add_subdirectory(src)

