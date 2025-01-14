add_library(RenderOpenGL)
add_library(Snowglobe::RenderOpenGL ALIAS RenderOpenGL)

target_include_directories(RenderOpenGL
    PRIVATE
    ${CMAKE_SOURCE_DIR}/libraries/include
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(RenderOpenGL
    PRIVATE
    glad
    glfw
    glm
    stb
    imgui
    Snowglobe::SnowCore
    Snowglobe::Render
)

file(GLOB RENDER_OPENGL_SOURCE *.cpp)
file(GLOB RENDER_OPENGL_HEADER *.hpp)

target_sources(RenderOpenGL
    PRIVATE
    ${RENDER_OPENGL_SOURCE}
    ${RENDER_OPENGL_HEADER}
)

add_subdirectory(Imgui)
add_subdirectory(Materials)