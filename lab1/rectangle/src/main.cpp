#include "Rectangle.h"
#include <iostream>

static Rectangle readRectangle(const char* prompt) {
    std::cout << prompt << " (x1 y1 x2 y2): ";
    Rectangle r;
    while (!(std::cin >> r)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invalid input, try again (x1 y1 x2 y2): ";
    }
    return r;
}

int main() {
    Rectangle a = readRectangle("Initial rectangle");
    int choice = -1;
    do {
        std::cout <<
            "\n===== Rectangle (Lab 1.3) =====\n"
            "1. Show rectangle\n"
            "2. Read rectangle (x1 y1 x2 y2)\n"
            "3. Move rectangle\n"
            "4. Resize rectangle\n"
            "5. Grow by 1 along each axis (++)\n"
            "6. Shrink by 1 along each axis (--)\n"
            "7. Union (bounding) with another rectangle (+)\n"
            "8. Intersection with another rectangle (-)\n"
            "9. Compare with another rectangle (== / !=)\n"
            "0. Exit\n> ";
        if (!(std::cin >> choice)) return 0;
        switch (choice) {
        case 1: std::cout << "a = " << a << " (w=" << a.width()
                          << ", h=" << a.height() << ")\n"; break;
        case 2: a = readRectangle("New rectangle"); break;
        case 3: {
            int dx, dy;
            std::cout << "dx dy: ";
            std::cin >> dx >> dy;
            a.move(dx, dy);
            std::cout << "a = " << a << "\n";
            break;
        }
        case 4: {
            int w, h;
            std::cout << "new width and height: ";
            std::cin >> w >> h;
            a.resize(w, h);
            std::cout << "a = " << a << "\n";
            break;
        }
        case 5: std::cout << "a++ = " << a++ << ", now a = " << a << "\n"; break;
        case 6: std::cout << "a-- = " << a-- << ", now a = " << a << "\n"; break;
        case 7: {
            Rectangle b = readRectangle("Second rectangle");
            std::cout << "a + b = " << (a + b) << "\n";
            a += b;
            std::cout << "after a += b: a = " << a << "\n";
            break;
        }
        case 8: {
            Rectangle b = readRectangle("Second rectangle");
            std::cout << "a - b = " << (a - b) << "\n";
            a -= b;
            std::cout << "after a -= b: a = " << a << "\n";
            break;
        }
        case 9: {
            Rectangle b = readRectangle("Second rectangle");
            std::cout << (a == b ? "a == b\n" : "a != b\n");
            break;
        }
        default: break;
        }
    } while (choice != 0);
    return 0;
}