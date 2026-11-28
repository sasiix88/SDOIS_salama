add_library(SetLib STATIC src/Set.cpp)
target_include_directories(SetLib PUBLIC src)

add_executable(SetApp src/main.cpp)
target_link_libraries(SetApp PRIVATE SetLib)

add_executable(TestSet tests/tst-Set.cpp)
target_link_libraries(TestSet PRIVATE SetLib ${GTEST_MAIN_TARGET})
gtest_discover_tests(TestSet)