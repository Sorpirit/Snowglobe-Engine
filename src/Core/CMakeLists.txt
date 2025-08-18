add_library(Core)
add_library(Snowglobe::Core ALIAS Core)

target_sources(Core 
    PRIVATE
        ECS/LifetimeSystem.cpp
        Assets/AssetManager.cpp
        FileTexture.cpp
        FileSystem.cpp
)

target_include_directories(Core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(Core
    PUBLIC
        cxx_setup
    PRIVATE
        glm
        stb
        nlohmann_json
        spdlog
        TracyClient
)