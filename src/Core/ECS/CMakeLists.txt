file(GLOB SNOW_CORE_ESC_SOURCE *.cpp)
file(GLOB SNOW_CORE_ESC_HEADER *.hpp)

target_sources(Core
    PRIVATE
    ${SNOW_CORE_ESC_SOURCE}
    ${SNOW_CORE_ESC_HEADER}
)

add_subdirectory(ECSTest)