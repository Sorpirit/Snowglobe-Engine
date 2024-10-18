option(BUILD_STATIC_LIBS "Build using static libraries" ON)
option(BUILD_SHARED_LIBS  "Build using shared libraries" OFF)

add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(glm)
add_subdirectory(stb)
add_subdirectory(imgui)
add_subdirectory(fastgltf)