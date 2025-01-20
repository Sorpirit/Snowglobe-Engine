file(GLOB SNOW_CORE_SOURCE *.cpp)
file(GLOB SNOW_CORE_HEADER *.hpp)

add_library(SnowCore ${SNOW_CORE_SOURCE} ${SNOW_CORE_HEADER})
add_library(Snowglobe::SnowCore ALIAS SnowCore)

target_include_directories(SnowCore
    PRIVATE
    ${CMAKE_SOURCE_DIR}/libraries/include
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(SnowCore
    PRIVATE
    glm
    stb
)

add_subdirectory(ECS)