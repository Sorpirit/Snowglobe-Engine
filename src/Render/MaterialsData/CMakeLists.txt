file(GLOB RENDER_MATERIALS_DATA_SOURCE *.cpp)
file(GLOB RENDER_MATERIALS_DATA_HEADER *.hpp)

target_sources(Render
    PRIVATE
    ${RENDER_MATERIALS_DATA_SOURCE}
    ${RENDER_MATERIALS_DATA_HEADER}
)