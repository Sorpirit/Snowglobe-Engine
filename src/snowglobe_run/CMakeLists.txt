add_executable(snowglobe_run main_gl.cpp)

find_package(Vulkan REQUIRED)

target_include_directories(snowglobe_run
    PRIVATE
    ${CMAKE_SOURCE_DIR}/libraries/include
)
target_link_libraries(snowglobe_run
    PRIVATE
    glad
    glfw
    Vulkan::Vulkan
    glm
    stb
    imgui
)