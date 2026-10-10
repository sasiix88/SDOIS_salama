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
    void resize(int newWidth, int newHeight) {
        x2 = x1 + newWidth;
        y2 = y1 + newHeight;
    }

    Rectangle& operator++() {
        ++x2; ++y2;
        return *this;
    }    
    Rectangle operator++(int) {
        Rectangle temp = *this;
        ++(*this);
        return temp;
    }

    Rectangle& operator--() {
        if (x2 > x1) --x2;
        if (y2 > y1) --y2;
        return *this;
    }    
    Rectangle operator--(int) {
        Rectangle temp = *this;
        --(*this);
        return temp;
    }
    
    Rectangle operator+(const Rectangle& other) const {
        return Rectangle(
            std::min(x1, other.x1),
            std::min(y1, other.y1),
            std::max(x2, other.x2),
            std::max(y1, other.y1)
        );
    }
    Rectangle& operator+=(const Rectangle& other) {
        *this = *this + other;
        return *this;
    }

    Rectangle operator-(const Rectangle& other) const {
        int newx1 = std::max(x1, other.x1);
        int newy1 = std::max(y1, other.y1);
        int newx2 = std::min(x2, other.x2);
        int newy2 = std::min(y2, other.y2);
        if (newx1 >= newx2 || newy1 >= newy2)
            return Rectangle(0, 0, 0, 0);
        return Rectangle(newx1, newy1, newx2, newy2);
    }
    Rectangle& operator-=(const Rectangle& other) {
        *this = *this - other;
        return *this;
    }

    void print() const {
        std::cout << "(" << x1 << "," << y1 << ") - ("
                   << x2 << "," << y2 << ")\n";
    }
};

int main() {
    Rectangle a(0, 0, 4, 3);
    Rectangle b(2, 1, 6, 5);

    a.print();
    b.print();

    Rectangle u = a + b;
    u.print();

    Rectangle i = a - b;
    i.print();

    ++a; 
    a.print();

    a--;
    a.print();

    a.move(10, 10);
    a.print();

    a.resize(2, 2);
    a.print();

    return 0;
}