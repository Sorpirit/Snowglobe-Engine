add_library(stb STATIC stb_implementation.cpp)
add_library(stb_inc INTERFACE)
target_include_directories(stb_inc INTERFACE ./)
target_link_libraries(stb PUBLIC stb_inc)