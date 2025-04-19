file(GLOB SNOW_ENGINE_SOURCE *.cpp)
file(GLOB SNOW_ENGINE_HEADER *.hpp)

file(GLOB SNOW_ENGINE_EDITOR_SOURCE Editor/*.cpp)
file(GLOB SNOW_ENGINE_EDITOR_HEADER Editor/*.hpp)

add_library(Engine ${SNOW_ENGINE_SOURCE} ${SNOW_ENGINE_HEADER} ${SNOW_ENGINE_EDITOR_SOURCE} ${SNOW_ENGINE_EDITOR_HEADER} "Editor/EngineInspectorSystem.cpp")
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

#assimp
target_link_libraries(Engine PRIVATE
        debug ${CMAKE_SOURCE_DIR}/libraries/bin/assimp/debug/assimp-vc143-mtd.lib
        optimized ${CMAKE_SOURCE_DIR}/libraries/bin/assimp/release/assimp-vc143-mt.lib)

#assimp dependency
target_link_libraries(Engine PRIVATE
        debug ${CMAKE_SOURCE_DIR}/libraries/bin/assimp/debug/zlibstaticd.lib
        optimized ${CMAKE_SOURCE_DIR}/libraries/bin/assimp/release/zlibstatic.lib)