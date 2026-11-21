add_library(RectangleLib STATIC src/Rectangle.cpp)
target_include_directories(RectangleLib PUBLIC src)

add_executable(RectangleApp src/main.cpp)
target_link_libraries(RectangleApp PRIVATE RectangleLib)

add_executable(TestRectangle tests/tst-Rectangle.cpp)
target_link_libraries(TestRectangle PRIVATE RectangleLib ${GTEST_MAIN_TARGET})
gtest_discover_tests(TestRectangle)