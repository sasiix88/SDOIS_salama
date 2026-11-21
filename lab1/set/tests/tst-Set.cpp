#include <gtest/gtest.h>
#include "Set.h"
#include <sstream>
#include <stdexcept>

TEST(Set, EmptyAndSize) {
    Set s;
    EXPECT_TRUE(s.isEmpty());
    EXPECT_EQ(s.size(), 0u);
    s.add("a");
    EXPECT_FALSE(s.isEmpty());
    EXPECT_EQ(s.size(), 1u);
}

TEST(Set, AddDoesNotRepeatElements) {
    Set s;
    s.add("a");
    s.add("a");
    s.add("{b, c}");
    s.add("{c, b}");                       // the same nested set, other order
    EXPECT_EQ(s.size(), 2u);
}

TEST(Set, AddRejectsWrongElements) {
    Set s;
    EXPECT_THROW(s.add(""), std::invalid_argument);
    EXPECT_THROW(s.add("a b"), std::invalid_argument);
    EXPECT_THROW(s.add("a,b"), std::invalid_argument);
    EXPECT_THROW(s.add("{a"), std::invalid_argument);
    EXPECT_TRUE(s.isEmpty());
}

TEST(Set, RemoveAndContains) {
    Set s("{a, b, {c, d}}");
    EXPECT_TRUE(s["a"]);
    EXPECT_TRUE(s["{d, c}"]);
    EXPECT_FALSE(s["c"]);                  // c is inside the nested set, not in s itself
    EXPECT_TRUE(s.remove("a"));
    EXPECT_FALSE(s.remove("a"));
    EXPECT_TRUE(s.remove("{c, d}"));
    EXPECT_EQ(s, Set("{b}"));
}

TEST(Set, ParseExampleFromAssignment) {
    Set s("{a, b, c, {a, b}, {}, {a, {c}}}");
    EXPECT_EQ(s.size(), 6u);
    EXPECT_TRUE(s["{a, b}"]);
    EXPECT_TRUE(s["{}"]);
    EXPECT_TRUE(s["{a, {c}}"]);
}

TEST(Set, ParseSpacesAndEmptySet) {
    EXPECT_TRUE(Set("{}").isEmpty());
    EXPECT_TRUE(Set("  {   }  ").isEmpty());
    EXPECT_EQ(Set(" { a ,b,{ c } } "), Set("{a, b, {c}}"));
    EXPECT_EQ(Set("{a, a, b}").size(), 2u);
}

TEST(Set, ParseRejectsWrongText) {
    EXPECT_THROW(Set(""), std::invalid_argument);
    EXPECT_THROW(Set("a"), std::invalid_argument);
    EXPECT_THROW(Set("{a"), std::invalid_argument);
    EXPECT_THROW(Set("a}"), std::invalid_argument);
    EXPECT_THROW(Set("{a,}"), std::invalid_argument);
    EXPECT_THROW(Set("{,a}"), std::invalid_argument);
    EXPECT_THROW(Set("{a}}"), std::invalid_argument);
    EXPECT_THROW(Set("{{a}"), std::invalid_argument);
    EXPECT_THROW(Set("{a b}"), std::invalid_argument);
    EXPECT_THROW(Set("{a}{b}"), std::invalid_argument);
}

TEST(Set, Union) {
    Set a("{a, b, {x}}"), b("{b, c, {x}, {y}}");
    EXPECT_EQ(a + b, Set("{a, b, c, {x}, {y}}"));
    EXPECT_EQ(a, Set("{a, b, {x}}"));      // operands are not changed
    a += b;
    EXPECT_EQ(a, Set("{a, b, c, {x}, {y}}"));
}

TEST(Set, Intersection) {
    Set a("{a, b, {x}, {z}}"), b("{b, c, {x}, {y}}");
    EXPECT_EQ(a * b, Set("{b, {x}}"));
    a *= b;
    EXPECT_EQ(a, Set("{b, {x}}"));
}

TEST(Set, Difference) {
    Set a("{a, b, {x}, {z}}"), b("{b, c, {x}, {y}}");
    EXPECT_EQ(a - b, Set("{a, {z}}"));
    a -= b;
    EXPECT_EQ(a, Set("{a, {z}}"));
}

TEST(Set, OperationsWithItselfAndWithEmptySet) {
    Set a("{a, {b}}"), empty;
    EXPECT_EQ(a + a, a);
    EXPECT_EQ(a * a, a);
    EXPECT_TRUE((a - a).isEmpty());
    EXPECT_EQ(a + empty, a);
    EXPECT_TRUE((a * empty).isEmpty());
    EXPECT_EQ(a - empty, a);
}

TEST(Set, PowerSet) {
    EXPECT_EQ(Set().powerSet(), Set("{{}}"));
    EXPECT_EQ(Set("{a, b}").powerSet(), Set("{{}, {a}, {b}, {a, b}}"));
    EXPECT_EQ(Set("{a, {b}}").powerSet(), Set("{{}, {a}, {{b}}, {a, {b}}}"));
    EXPECT_EQ(Set("{a, b, c, d, e}").powerSet().size(), 32u);
}

TEST(Set, PowerSetOfTooLargeSetThrows) {
    Set big;
    for (int i = 0; i < 17; ++i) big.add("x" + std::to_string(i));
    EXPECT_THROW(big.powerSet(), std::length_error);
}

TEST(Set, CopyIsIndependent) {
    Set a("{a, {b}}");
    Set copy(a);
    copy.add("z");
    EXPECT_EQ(a, Set("{a, {b}}"));
    Set other;
    other = a;
    other.remove("a");
    EXPECT_EQ(a.size(), 2u);
    EXPECT_EQ(other.size(), 1u);
}

TEST(Set, EqualityIgnoresOrder) {
    EXPECT_TRUE(Set("{a, b, {c, d}}") == Set("{{d, c}, b, a}"));
    EXPECT_FALSE(Set("{a, b}") == Set("{a, c}"));
    EXPECT_TRUE(Set("{a, b}") != Set("{a}"));
    EXPECT_FALSE(Set("{a}") != Set("{a}"));
    EXPECT_TRUE(Set("{a}") != Set("{{a}}"));
}

TEST(Set, StreamOutput) {
    std::ostringstream oss;
    oss << Set("{c, a, {b}}");
    EXPECT_EQ(oss.str(), "{a, c, {b}}");
    EXPECT_EQ(Set().toString(), "{}");
}

TEST(Set, StreamInput) {
    std::istringstream iss("  {a, {b}}  {c}");
    Set first, second;
    iss >> first >> second;
    EXPECT_EQ(first, Set("{a, {b}}"));
    EXPECT_EQ(second, Set("{c}"));
}

TEST(Set, StreamInputWithWrongTextSetsFailbit) {
    std::istringstream bad("{a, b c}");
    Set s("{keep}");
    bad >> s;
    EXPECT_TRUE(bad.fail());
    EXPECT_EQ(s, Set("{keep}"));

    std::istringstream unfinished("{a, {b}");
    unfinished >> s;
    EXPECT_TRUE(unfinished.fail());

    std::istringstream empty("");
    empty >> s;
    EXPECT_TRUE(empty.fail());
}