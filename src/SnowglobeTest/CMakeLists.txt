file(GLOB SNOW_TEST_SOURCE *.cpp)
file(GLOB SNOW_TEST_HEADER *.hpp)

add_executable(SnowglobeTest ${SNOW_TEST_SOURCE} ${SNOW_TEST_HEADER})

#target_compile_options(SnowglobeTest PRIVATE -fsanitize=address)
#target_link_directories(SnowglobeTest PRIVATE "$ENV{ProgramFiles\(x86\)}/Microsoft Visual Studio/2022/Community/VC/Tools/Llvm/x64/lib/clang/18/lib/windows")
#target_link_libraries(SnowglobeTest PRIVATE clang_rt.asan_dynamic-x86_64 clang_rt.asan_dynamic_runtime_thunk-x86_64)
#target_link_options(SnowglobeTest PRIVATE /wholearchive:clang_rt.asan_dynamic_runtime_thunk-x86_64.lib)

target_include_directories(SnowglobeTest
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(SnowglobeTest
    PRIVATE
    glm
    nlohmann_json
    assimp
    Snowglobe::Core
    Snowglobe::Render
    Snowglobe::Engine
)

add_subdirectory(Tests)