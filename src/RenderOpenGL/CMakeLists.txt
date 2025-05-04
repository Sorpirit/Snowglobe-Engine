file(GLOB RENDER_OPENGL_SOURCE *.cpp)
file(GLOB RENDER_OPENGL_HEADER *.hpp)

add_library(RenderOpenGL)
add_library(Snowglobe::RenderOpenGL ALIAS RenderOpenGL)

target_include_directories(RenderOpenGL
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
    glslang
    glslang-default-resource-limits
    Snowglobe::Core
    Snowglobe::Render
)

 target_sources(RenderOpenGL
     PRIVATE
     ${RENDER_OPENGL_SOURCE}
     PUBLIC
     ${RENDER_OPENGL_HEADER}
 )

add_subdirectory(Imgui)
add_subdirectory(Materials)