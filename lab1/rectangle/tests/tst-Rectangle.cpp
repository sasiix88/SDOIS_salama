#include <gtest/gtest.h>
#include "Rectangle.h"
#include <sstream>

TEST(Rectangle, CtorNormalizesCoordinates) {
    Rectangle r(5, 7, 1, 2);           // corners given "unsorted"
    EXPECT_EQ(r.getX1(), 1); EXPECT_EQ(r.getY1(), 2);
    EXPECT_EQ(r.getX2(), 5); EXPECT_EQ(r.getY2(), 7);
}

TEST(Rectangle, Move) {
    Rectangle r(0, 0, 3, 4);
    r.move(2, -1);
    EXPECT_EQ(r, Rectangle(2, -1, 5, 3));   // regression: y2 must move too
    EXPECT_EQ(r.width(), 3);
    EXPECT_EQ(r.height(), 4);
}

TEST(Rectangle, SizeAndResize) {
    Rectangle r(1, 1, 4, 6);
    EXPECT_EQ(r.width(), 3);
    EXPECT_EQ(r.height(), 5);
    EXPECT_FALSE(r.isEmpty());
    EXPECT_TRUE(Rectangle(2, 2, 2, 5).isEmpty());
    r.resize(10, 2);
    EXPECT_EQ(r, Rectangle(1, 1, 11, 3));
}

TEST(Rectangle, IncrementDecrement) {
    Rectangle r(0, 0, 3, 5);
    Rectangle old = r++;
    EXPECT_EQ(old, Rectangle(0, 0, 3, 5));
    EXPECT_EQ(r, Rectangle(0, 0, 4, 6));

    old = r--;                            // post-decrement returns old value
    EXPECT_EQ(old, Rectangle(0, 0, 4, 6));
    EXPECT_EQ(r, Rectangle(0, 0, 3, 5));
    --r; --r; --r; --r; --r;             // must clamp at the bottom-left corner
    EXPECT_EQ(r, Rectangle(0, 0, 0, 0));
    --r;
    EXPECT_EQ(r, Rectangle(0, 0, 0, 0)); // never goes negative
}

TEST(Rectangle, Union) {
    Rectangle a(0, 0, 2, 2), b(1, 1, 4, 3);
    EXPECT_EQ(a + b, Rectangle(0, 0, 4, 3)); // regression: top edge was wrong
    Rectangle c(10, 10, 12, 12);
    EXPECT_EQ(a + c, Rectangle(0, 0, 12, 12));

    a += b;
    EXPECT_EQ(a, Rectangle(0, 0, 4, 3));
}

TEST(Rectangle, Intersection) {
    Rectangle a(0, 0, 2, 2), b(1, 1, 4, 3);
    EXPECT_EQ(a - b, Rectangle(1, 1, 2, 2));
    Rectangle c(10, 10, 12, 12);          // disjoint -> empty rectangle
    EXPECT_EQ(a - c, Rectangle(0, 0, 0, 0));
    EXPECT_TRUE((a - c).isEmpty());

    a -= b;
    EXPECT_EQ(a, Rectangle(1, 1, 2, 2));
}

TEST(Rectangle, EqualityAndStreams) {
    Rectangle a(0, 0, 1, 1);
    Rectangle b(1, 1, 0, 0);              // normalizes to the same rectangle
    EXPECT_TRUE(a == b);
    EXPECT_FALSE(a != b);
    Rectangle c(0, 0, 1, 2);
    EXPECT_TRUE(a != c);

    std::ostringstream oss;
    oss << a;
    EXPECT_EQ(oss.str(), "(0,0) - (1,1)");

    std::istringstream iss("2 3 5 9");
    Rectangle d;
    iss >> d;
    EXPECT_EQ(d, Rectangle(2, 3, 5, 9));
}