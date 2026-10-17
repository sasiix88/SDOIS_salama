#include "Rectangle.h"

#include <algorithm>
#include <iostream>

void Rectangle::normalize() {
    if (x1 > x2) std::swap(x1, x2);
    if (y1 > y2) std::swap(y1, y2);
}

Rectangle::Rectangle(int x1, int y1, int x2, int y2)
    : x1(x1), y1(y1), x2(x2), y2(y2) {
    normalize();
}

int Rectangle::getX1() const { return x1; }
int Rectangle::getX2() const { return x2; }
int Rectangle::getY1() const { return y1; }
int Rectangle::getY2() const { return y2; }

int Rectangle::width()  const { return x2 - x1; }
int Rectangle::height() const { return y2 - y1; }
bool Rectangle::isEmpty() const { return x1 >= x2 || y1 >= y2; }

void Rectangle::move(int dx, int dy) {
    x1 += dx; x2 += dx;
    y1 += dy; y2 += dy;          // FIX: was "y1 += dy; y1 += dy;"
}

void Rectangle::resize(int newWidth, int newHeight) {
    x2 = x1 + newWidth;
    y2 = y1 + newHeight;
}

// Grow by 1 along each axis (bottom-left corner stays in place).
Rectangle& Rectangle::operator++() {
    ++x2; ++y2;
    return *this;
}
Rectangle Rectangle::operator++(int) {
    Rectangle temp = *this;
    ++(*this);
    return temp;
}

// Shrink by 1 along each axis, never past the bottom-left corner.
Rectangle& Rectangle::operator--() {
    if (x2 > x1) --x2;
    if (y2 > y1) --y2;
    return *this;
}
Rectangle Rectangle::operator--(int) {
    Rectangle temp = *this;
    --(*this);
    return temp;
}

// Smallest rectangle containing both.
Rectangle Rectangle::operator+(const Rectangle& other) const {
    return Rectangle(
        std::min(x1, other.x1),
        std::min(y1, other.y1),
        std::max(x2, other.x2),
        std::max(y2, other.y2)   // FIX: was std::max(y1, other.y1)
    );
}
Rectangle& Rectangle::operator+=(const Rectangle& other) {
    *this = *this + other;
    return *this;
}

// Intersection; a disjoint pair yields the empty rectangle (0,0,0,0).
Rectangle Rectangle::operator-(const Rectangle& other) const {
    int newx1 = std::max(x1, other.x1);
    int newy1 = std::max(y1, other.y1);
    int newx2 = std::min(x2, other.x2);
    int newy2 = std::min(y2, other.y2);
    if (newx1 >= newx2 || newy1 >= newy2)
        return Rectangle(0, 0, 0, 0);
    return Rectangle(newx1, newy1, newx2, newy2);
}
Rectangle& Rectangle::operator-=(const Rectangle& other) {
    *this = *this - other;
    return *this;
}

// NEW: equality required by the assignment.
bool Rectangle::operator==(const Rectangle& other) const {
    return x1 == other.x1 && y1 == other.y1 &&
           x2 == other.x2 && y2 == other.y2;
}
bool Rectangle::operator!=(const Rectangle& other) const {
    return !(*this == other);
}

// NEW: stream output instead of print() — keeps the class independent
// of any particular user interface.
std::ostream& operator<<(std::ostream& os, const Rectangle& r) {
    return os << "(" << r.x1 << "," << r.y1 << ") - ("
              << r.x2 << "," << r.y2 << ")";
}

// NEW: stream input: reads "x1 y1 x2 y2" (whitespace-separated).
std::istream& operator>>(std::istream& is, Rectangle& r) {
    int a, b, c, d;
    if (is >> a >> b >> c >> d)
        r = Rectangle(a, b, c, d);
    return is;
}
