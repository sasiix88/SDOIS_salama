#include "Set.h"
#include <iostream>
#include <limits>
#include <string>

static void skipLine() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static Set readSet(const char* prompt) {
    std::cout << prompt << " (for example {a, b, {a, b}, {}}): ";
    Set s;
    while (!(std::cin >> s)) {
        if (std::cin.eof()) return s;       // the input has ended, stop asking
        skipLine();
        std::cout << "Invalid set, try again: ";
    }
    skipLine();
    return s;
}

static std::string readElement() {
    std::cout << "Element (a word or a set like {a, b}): ";
    std::string element;
    std::getline(std::cin, element);
    return element;
}

static void printMenu() {
    std::cout <<
        "\n===== Set (Lab 1.4.1) =====\n"
        "1. Show sets A and B\n"
        "2. Read set A from text\n"
        "3. Read set B from text\n"
        "4. Add an element to A\n"
        "5. Remove an element from A\n"
        "6. Check whether an element belongs to A (A[x])\n"
        "7. Union (A + B)\n"
        "8. Intersection (A * B)\n"
        "9. Difference (A - B)\n"
        "10. A += B\n"
        "11. A *= B\n"
        "12. A -= B\n"
        "13. Power set of A\n"
        "14. Compare A and B (== / !=)\n"
        "0. Exit\n> ";
}

static void showSets(const Set& a, const Set& b) {
    std::cout << "A = " << a << (a.isEmpty() ? " (empty)" : "") << ", size " << a.size() << "\n";
    std::cout << "B = " << b << (b.isEmpty() ? " (empty)" : "") << ", size " << b.size() << "\n";
}

static void addElement(Set& a) {
    skipLine();
    try {
        a.add(readElement());
        std::cout << "A = " << a << "\n";
    } catch (const std::exception& error) {
        std::cout << "Error: " << error.what() << "\n";
    }
}

static void removeElement(Set& a) {
    skipLine();
    try {
        bool removed = a.remove(readElement());
        std::cout << (removed ? "Removed" : "No such element") << ", A = " << a << "\n";
    } catch (const std::exception& error) {
        std::cout << "Error: " << error.what() << "\n";
    }
}

static void checkElement(const Set& a) {
    skipLine();
    try {
        std::cout << (a[readElement()] ? "The element belongs to A\n" : "The element does not belong to A\n");
    } catch (const std::exception& error) {
        std::cout << "Error: " << error.what() << "\n";
    }
}

static void showPowerSet(const Set& a) {
    try {
        std::cout << "P(A) = " << a.powerSet() << "\n";
    } catch (const std::exception& error) {
        std::cout << "Error: " << error.what() << "\n";
    }
}

int main() {
    Set a, b;
    int choice = -1;
    do {
        printMenu();
        if (!(std::cin >> choice)) return 0;
        switch (choice) {
        case 1: showSets(a, b); break;
        case 2: skipLine(); a = readSet("Set A"); break;
        case 3: skipLine(); b = readSet("Set B"); break;
        case 4: addElement(a); break;
        case 5: removeElement(a); break;
        case 6: checkElement(a); break;
        case 7: std::cout << "A + B = " << (a + b) << "\n"; break;
        case 8: std::cout << "A * B = " << (a * b) << "\n"; break;
        case 9: std::cout << "A - B = " << (a - b) << "\n"; break;
        case 10: a += b; std::cout << "A = " << a << "\n"; break;
        case 11: a *= b; std::cout << "A = " << a << "\n"; break;
        case 12: a -= b; std::cout << "A = " << a << "\n"; break;
        case 13: showPowerSet(a); break;
        case 14: std::cout << (a == b ? "A == B\n" : "A != B\n"); break;
        default: break;
        }
    } while (choice != 0);
    return 0;
}