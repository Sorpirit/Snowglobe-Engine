add_executable(SnowglobeTest SnowglobeMain.cpp)

target_include_directories(SnowglobeTest
    PRIVATE
    ${CMAKE_SOURCE_DIR}/libraries/include
)

target_link_libraries(SnowglobeTest
    PRIVATE
    glm
    Snowglobe::SnowCore
    Snowglobe::SnowEngine
    Snowglobe::Render
)