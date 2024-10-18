add_library(glad gl.c)

target_include_directories(glad
    PRIVATE
    ${CMAKE_SOURCE_DIR}/libraries/include
)