#include "Set.h"
#include <iostream>
#include <stdexcept>
#include <vector>

const std::size_t MAX_POWER_SET_SIZE = 16;
const std::string WHITESPACE = " \t\r\n";

static std::string trim(const std::string& text) {
    std::size_t first = text.find_first_not_of(WHITESPACE);
    if (first == std::string::npos) return "";
    std::size_t last = text.find_last_not_of(WHITESPACE);
    return text.substr(first, last - first + 1);
}

static bool isSetText(const std::string& text) {
    return !text.empty() && text[0] == '{';
}

static std::vector<std::string> splitElements(const std::string& text) {
    std::vector<std::string> parts;
    std::string current;
    int depth = 0;
    for (char symbol : text) {
        if (symbol == '{') ++depth;
        if (symbol == '}') --depth;
        if (depth < 0) throw std::invalid_argument("Too many closing braces");
        if (symbol == ',' && depth == 0) {
            parts.push_back(current);
            current.clear();
        } else {
            current += symbol;
        }
    }
    if (depth != 0) throw std::invalid_argument("Braces do not match");
    parts.push_back(current);
    return parts;
}

std::string Set::normalize(const std::string& element) {
    std::string text = trim(element);
    if (text.empty()) throw std::invalid_argument("Empty element");
    if (isSetText(text)) return Set(text).toString();
    if (text.find_first_of(" \t{},") != std::string::npos)
        throw std::invalid_argument("A word cannot contain spaces, braces or commas: " + text);
    return text;
}

Set::Set() {}
Set::Set(const std::string& text) {
    std::string trimmed = trim(text);
    if (trimmed.size() < 2 || trimmed.front() != '{' || trimmed.back() != '}')
        throw std::invalid_argument("A set must look like {a, b, {c}}");
    std::string inside = trim(trimmed.substr(1, trimmed.size() - 2));
    if (inside.empty()) return;
    for (const std::string& part : splitElements(inside))
        add(part);
}

bool Set::isEmpty() const { return elements.empty(); }
std::size_t Set::size() const { return elements.size(); }

void Set::add(const std::string& element) {
    elements.insert(normalize(element));              // a std::set ignores repeated elements
}

bool Set::remove(const std::string& element) {
    return elements.erase(normalize(element)) > 0;
}

bool Set::operator[](const std::string& element) const {
    return elements.count(normalize(element)) > 0;
}

Set Set::operator+(const Set& other) const {
    Set result = *this;
    for (const std::string& element : other.elements)
        result.elements.insert(element);
    return result;
}

Set Set::operator*(const Set& other) const {
    Set result;
    for (const std::string& element : elements)
        if (other.elements.count(element) > 0)
            result.elements.insert(element);
    return result;
}

Set Set::operator-(const Set& other) const {
    Set result;
    for (const std::string& element : elements)
        if (other.elements.count(element) == 0)
            result.elements.insert(element);
    return result;
}

Set& Set::operator+=(const Set& other) { *this = *this + other; return *this; }
Set& Set::operator*=(const Set& other) { *this = *this * other; return *this; }
Set& Set::operator-=(const Set& other) { *this = *this - other; return *this; }

Set Set::powerSet() const {
    if (elements.size() > MAX_POWER_SET_SIZE)
        throw std::length_error("The power set is too large");
    std::vector<std::string> items(elements.begin(), elements.end());
    Set result;
    unsigned long subsetCount = 1UL << items.size();
    for (unsigned long mask = 0; mask < subsetCount; ++mask) {
        Set subset;
        for (std::size_t i = 0; i < items.size(); ++i)
            if (mask & (1UL << i))
                subset.elements.insert(items[i]);
        result.elements.insert(subset.toString());
    }
    return result;
}

std::string Set::toString() const {
    std::string text = "{";
    for (const std::string& element : elements) {
        if (text.size() > 1) text += ", ";
        text += element;
    }
    return text + "}";
}

bool Set::operator==(const Set& other) const { return elements == other.elements; }
bool Set::operator!=(const Set& other) const { return !(*this == other); }

std::ostream& operator<<(std::ostream& os, const Set& s) {
    return os << s.toString();
}

std::istream& operator>>(std::istream& is, Set& s) {
    std::string text;
    char symbol;
    int depth = 0;
    is >> std::ws;
    while (is.get(symbol)) {
        text += symbol;
        if (symbol == '{') ++depth;
        if (symbol == '}') --depth;
        if (depth <= 0) break;
    }
    try {
        s = Set(text);
    } catch (const std::invalid_argument&) {
        is.setstate(std::ios::failbit);
    }
    return is;
}