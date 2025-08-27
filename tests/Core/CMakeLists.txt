include(GoogleTest)

add_executable(CoreTests)

target_sources(CoreTests
    PRIVATE
        CoreMain.cpp
        ECS/ECSSimpleTest.cpp
)

target_link_libraries(CoreTests
    PRIVATE
        GTest::gtest
        Snowglobe::Core
)

add_test(CoreTests CoreTests)