#ifndef SET_H
#define SET_H

#include <cstddef>
#include <iosfwd>
#include <set>
#include <string>

class Set {
private:
    std::set<std::string> elements;

    static std::string normalize(const std::string& element);

public:
    Set();
    explicit Set(const std::string& text);                   // from text like "{a, b, {a, b}, {}}"
    Set(const Set& other) = default;                         // copy constructor
    Set& operator=(const Set& other) = default;              // copy assignment

    bool isEmpty() const;
    std::size_t size() const;                                // cardinality
    void add(const std::string& element);
    bool remove(const std::string& element);
    bool operator[](const std::string& element) const;       // true if the element belongs to the set

    Set  operator+(const Set& other) const;                  // union
    Set& operator+=(const Set& other);
    Set  operator*(const Set& other) const;                  // intersection
    Set& operator*=(const Set& other);
    Set  operator-(const Set& other) const;                  // difference
    Set& operator-=(const Set& other);

    Set powerSet() const;
    std::string toString() const;

    bool operator==(const Set& other) const;
    bool operator!=(const Set& other) const;
};

std::ostream& operator<<(std::ostream& os, const Set& s);
std::istream& operator>>(std::istream& is, Set& s);

#endif