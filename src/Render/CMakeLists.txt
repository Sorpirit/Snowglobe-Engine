add_library(Render)
add_library(Snowglobe::Render ALIAS Render)

target_include_directories(Render
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Render
    PRIVATE
    glm
    stb
    glfw
    Snowglobe::Core
    nlohmann_json
    assimp
)

file(GLOB RENDER_SOURCE *.cpp)
file(GLOB RENDER_HEADER *.hpp)

target_sources(Render
    PRIVATE
    ${RENDER_SOURCE}
    ${RENDER_HEADER}
)

add_subdirectory(GLFW)
add_subdirectory(MaterialsData)