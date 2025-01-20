add_executable(ECSTest ECSTest.cpp ECSTest.hpp)

target_link_libraries(ECSTest
    PRIVATE
    glm
    Snowglobe::SnowCore
)