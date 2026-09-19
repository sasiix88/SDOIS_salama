#include<iostream>
#include<algorithm>

class Rectangle {
private:
    int x, y;
    int width, height;
public:
    Rectangle(int x, int y, int W, int H)
        : x(x), y(y), width(W), height(H) {}
    
    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
};
