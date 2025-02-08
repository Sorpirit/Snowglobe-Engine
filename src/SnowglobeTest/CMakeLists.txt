file(GLOB SNOW_TEST_SOURCE *.cpp)
file(GLOB SNOW_TEST_HEADER *.hpp)

add_executable(SnowglobeTest ${SNOW_TEST_SOURCE} ${SNOW_TEST_HEADER})

target_compile_options(SnowglobeTest PRIVATE -fsanitize=address)
#target_link_directories(SnowglobeTest PRIVATE "C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\VC\\Tools\\Llvm\\lib\\clang\\17\\lib\\windows")
#target_link_libraries(SnowglobeTest PRIVATE clang_rt.asan_dynamic-x86_64 clang_rt.asan_dynamic_runtime_thunk-x86_64)
#target_link_options(SnowglobeTest PRIVATE /wholearchive:clang_rt.asan_dynamic_runtime_thunk-x86_64.lib)

target_include_directories(SnowglobeTest
    PRIVATE
    ${CMAKE_SOURCE_DIR}/libraries/include
)

target_link_libraries(SnowglobeTest
    PRIVATE
    glm
    Snowglobe::Core
    Snowglobe::Engine
    Snowglobe::Render
)