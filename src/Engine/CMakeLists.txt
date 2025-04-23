file(GLOB SNOW_ENGINE_SOURCE *.cpp)
file(GLOB SNOW_ENGINE_HEADER *.hpp)

file(GLOB SNOW_ENGINE_EDITOR_SOURCE Editor/*.cpp)
file(GLOB SNOW_ENGINE_EDITOR_HEADER Editor/*.hpp)

add_library(Engine ${SNOW_ENGINE_SOURCE} ${SNOW_ENGINE_HEADER} ${SNOW_ENGINE_EDITOR_SOURCE} ${SNOW_ENGINE_EDITOR_HEADER} "Editor/EngineInspectorSystem.cpp")
add_library(Snowglobe::Engine ALIAS Engine)

target_include_directories(Engine
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Engine
    PRIVATE
    assimp
    glm
    Snowglobe::Core
    Snowglobe::Render
    Snowglobe::RenderOpenGL
    imgui
    nlohmann_json
)