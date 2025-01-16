add_library(stb STATIC stb_implementation.cpp stb_image.hpp)

target_include_directories(stb
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
