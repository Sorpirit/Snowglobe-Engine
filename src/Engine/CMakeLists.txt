file(GLOB SNOW_ENGINE_SOURCE *.cpp)
file(GLOB SNOW_ENGINE_HEADER *.hpp)

add_library(Engine ${SNOW_ENGINE_SOURCE} ${SNOW_ENGINE_HEADER} "ComponentEditorSystem.cpp")
add_library(Snowglobe::Engine ALIAS Engine)

target_include_directories(Engine
    PRIVATE
    ${CMAKE_SOURCE_DIR}/libraries/include
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Engine
    PRIVATE
    glm
    Snowglobe::Core
    Snowglobe::Render
    Snowglobe::RenderOpenGL
    imgui
)