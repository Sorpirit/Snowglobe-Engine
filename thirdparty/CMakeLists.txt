option(BUILD_STATIC_LIBS "Build using static libraries" ON)
option(BUILD_SHARED_LIBS  "Build using shared libraries" OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(glad)
set_property(TARGET glad PROPERTY FOLDER "thirdparty")

add_subdirectory(glfw)
#Has ist own folder

add_subdirectory(glm)
set_property(TARGET glm_static PROPERTY FOLDER "thirdparty")

add_subdirectory(stb)
set_property(TARGET stb PROPERTY FOLDER "thirdparty")

add_subdirectory(imgui)
set_property(TARGET imgui PROPERTY FOLDER "thirdparty")

add_subdirectory(tracy)
set_property(TARGET TracyClient PROPERTY FOLDER "thirdparty")

add_subdirectory(fastgltf)
set_property(TARGET fastgltf PROPERTY FOLDER "thirdparty")