file(GLOB RENDER_VULKAN_SOURCE *.cpp)
file(GLOB RENDER_VULKAN_HEADER *.hpp)

add_library(RenderVulkan ${RENDER_VULKAN_SOURCE} ${RENDER_VULKAN_HEADER})
add_library(Snowglobe::RenderVulkan ALIAS RenderVulkan)

find_package(Vulkan REQUIRED)

target_include_directories(RenderVulkan
    PRIVATE
    ${CMAKE_SOURCE_DIR}/libraries/include
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(RenderVulkan
    PRIVATE
    glfw
    Vulkan::Vulkan
    glm
    stb
    imgui
    Snowglobe::SnowCore
    Snowglobe::Render
)