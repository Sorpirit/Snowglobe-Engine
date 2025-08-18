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

target_compile_options(Core PRIVATE
        $<$<OR:$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:GNU>>: -Wall -Wpedantic -Wextra -Werror>
        $<$<CXX_COMPILER_ID:MSVC>: /WX /W3 /wd4267 /EHsc>)