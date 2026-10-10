#include<iostream>
#include<algorithm>
#include<Rectangle.cpp>

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