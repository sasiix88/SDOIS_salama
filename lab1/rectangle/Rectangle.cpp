#include<iostream>
#include<algorithm>

class Rectangle {
private:
    int x1, y1;
    int x2, y2;

    void normalize() {
        if (x1 > x2) std::swap(x1, x2);
        if (y1 > y2) std::swap(y1, y2);
    }
public:
    Rectangle(int x1=0, int y1=0, int x2=0, int y2=0)
        : x1(x1), y1(y1), x2(x2), y2(y2) {
            normalize();
        }
    
    int getX1() const { return x1; }
    int getX2() const { return x2; }
    int getY1() const { return y1; }
    int getY2() const { return y2; }

    int width() const { return x2 - x1; }
    int height() const { return y2 - y1; }
    bool isEmpty() const { return x1 >= x2 || y1 >= y2; }

    void move(int dx, int dy) {
        x1 += dx; x2 += dx;
        y1 += dy; y1 += dy;
    }
};
