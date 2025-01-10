file(GLOB RENDER_SOURCE *.cpp)
file(GLOB RENDER_HEADER *.hpp)

add_library(Render ${RENDER_SOURCE} ${RENDER_HEADER} GLFW/WindowGLFW.cpp GLFW/WindowGLFW.hpp GLFW/InputGLFW.cpp GLFW/InputGLFW.hpp)
add_library(Snowglobe::Render ALIAS Render)

target_include_directories(Render
    PRIVATE
    ${CMAKE_SOURCE_DIR}/libraries/include
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Render
    PRIVATE
    glm
    stb
    glfw
    Snowglobe::SnowCore
)