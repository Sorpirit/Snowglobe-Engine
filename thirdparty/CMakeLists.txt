set(ASSIMP_BUILD_TESTS OFF CACHE INTERNAL "")
set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE INTERNAL "")
set(ASSIMP_NO_EXPORT ON CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE INTERNAL "")
set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
add_subdirectory(assimp)
set_property(TARGET assimp PROPERTY FOLDER "thirdparty")

add_subdirectory(fastgltf)
set_property(TARGET fastgltf PROPERTY FOLDER "thirdparty")

add_subdirectory(glad)
set_property(TARGET glad PROPERTY FOLDER "thirdparty")

add_subdirectory(glfw)
#set_property - Has ist own folder

add_subdirectory(glm)
set_property(TARGET glm PROPERTY FOLDER "thirdparty")

set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(json)
set_property(TARGET nlohmann_json PROPERTY FOLDER "thirdparty")

add_subdirectory(spdlog)
set_property(TARGET spdlog PROPERTY FOLDER "thirdparty")

add_subdirectory(stb)
set_property(TARGET stb PROPERTY FOLDER "thirdparty")

add_subdirectory(tracy)
set_property(TARGET TracyClient PROPERTY FOLDER "thirdparty")

# Setup libraries that does not have a CMakeLists.txt file

# ImGui
find_package(Vulkan REQUIRED)

add_library(imgui 
    imgui/imgui.cpp imgui/imgui_demo.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp imgui/backends/imgui_impl_opengl3.cpp imgui/backends/imgui_impl_vulkan.cpp
)

target_include_directories(imgui 
    PUBLIC 
    imgui
    PRIVATE
    imgui/backends
)

target_link_libraries(imgui 
    PRIVATE  
    glfw
    Vulkan::Vulkan
)

set_property(TARGET imgui PROPERTY FOLDER "thirdparty")