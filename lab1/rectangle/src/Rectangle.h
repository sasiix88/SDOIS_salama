#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <iosfwd>   // forward declarations of istream/ostream (keeps the header light)

/**
 * @brief Axis-aligned rectangle with integer vertex coordinates,
 * stored as normalized bottom-left (x1,y1) and top-right (x2,y2).
 */
class Rectangle {
private:
    int x1, y1;
    int x2, y2;

    void normalize();

public:
    Rectangle(int x1 = 0, int y1 = 0, int x2 = 0, int y2 = 0);

    int getX1() const;
    int getX2() const;
    int getY1() const;
    int getY2() const;

    int width() const;
    int height() const;
    bool isEmpty() const;

    void move(int dx, int dy);
    void resize(int newWidth, int newHeight);

    Rectangle& operator++();
    Rectangle  operator++(int);
    Rectangle& operator--();
    Rectangle  operator--(int);

    Rectangle  operator+(const Rectangle& other) const;
    Rectangle& operator+=(const Rectangle& other);
    Rectangle  operator-(const Rectangle& other) const;
    Rectangle& operator-=(const Rectangle& other);

    bool operator==(const Rectangle& other) const;
    bool operator!=(const Rectangle& other) const;

    friend std::ostream& operator<<(std::ostream& os, const Rectangle& r);
    friend std::istream& operator>>(std::istream& is, Rectangle& r);
};

#endif