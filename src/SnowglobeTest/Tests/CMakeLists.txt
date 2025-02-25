file(GLOB TESTS_SOURCE *.cpp)
file(GLOB TESTS_HEADER *.hpp)

target_sources(SnowglobeTest
        PRIVATE
        ${TESTS_SOURCE}
        ${TESTS_HEADER}
)
