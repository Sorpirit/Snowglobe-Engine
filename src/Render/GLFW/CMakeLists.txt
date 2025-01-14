file(GLOB RENDER_GLFW_SOURCE *.cpp)
file(GLOB RENDER_GLFW_HEADER *.hpp)

target_sources(Render
    PRIVATE
    ${RENDER_GLFW_SOURCE}
    ${RENDER_GLFW_HEADER}
)