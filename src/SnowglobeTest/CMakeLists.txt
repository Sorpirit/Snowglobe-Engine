file(GLOB SNOW_TEST_SOURCE *.cpp)
file(GLOB SNOW_TEST_HEADER *.hpp)

add_executable(SnowglobeTest ${SNOW_TEST_SOURCE} ${SNOW_TEST_HEADER})

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