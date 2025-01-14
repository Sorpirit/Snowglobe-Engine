file(GLOB SNOW_ENGINE_SOURCE *.cpp)
file(GLOB SNOW_ENGINE_HEADER *.hpp)

add_library(SnowEngine ${SNOW_ENGINE_SOURCE} ${SNOW_ENGINE_HEADER})
add_library(Snowglobe::SnowEngine ALIAS SnowEngine)

target_include_directories(SnowEngine
    PRIVATE
    ${CMAKE_SOURCE_DIR}/libraries/include
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(SnowEngine
    PRIVATE
    glm
    Snowglobe::SnowCore
    Snowglobe::Render
    Snowglobe::RenderOpenGL
    imgui
)