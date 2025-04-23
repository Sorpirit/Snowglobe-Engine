file(GLOB SNOW_CORE_SOURCE *.cpp)
file(GLOB SNOW_CORE_HEADER *.hpp)

add_library(Core ${SNOW_CORE_SOURCE} ${SNOW_CORE_HEADER})
add_library(Snowglobe::Core ALIAS Core)

target_include_directories(Core
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Core
    PRIVATE
    glm
    stb
    nlohmann_json
)

add_subdirectory(ECS)
add_subdirectory(Serialization)
add_subdirectory(Assets)