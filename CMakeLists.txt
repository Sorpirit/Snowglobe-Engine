cmake_minimum_required(VERSION 3.24..3.30)
project(Snowglobe VERSION 1.0
        DESCRIPTION "Snowglobe. Toy game engine!"
        LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)
endif()

add_library(cxx_setup INTERFACE)
target_compile_options(cxx_setup INTERFACE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>: -Wall -Wpedantic -Wextra>
        $<$<CXX_COMPILER_ID:MSVC>: /WX /W3 /wd4267 /EHsc>)
target_compile_features(cxx_setup INTERFACE cxx_std_23)

add_subdirectory(thirdparty)

#add_compile_options(-fsanitize=address)
#add_link_options(-fsanitize=address)

add_subdirectory(src)

