#include <lib/interpreter.h>
#include <gtest/gtest.h>
#include <sstream>

// Test absolute value
TEST(NumberFunctionsTestSuite, Abs) {
    std::string code = R"(
        print(abs(-5))
    )";
    std::string expected = "5";

    std::istringstream input(code);
    std::ostringstream output;
    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

// Test ceiling
TEST(NumberFunctionsTestSuite, Ceil) {
    std::string code = R"(
        print(ceil(3.2))
    )";
    std::string expected = "4";

    std::istringstream input(code);
    std::ostringstream output;
    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

// Test floor
TEST(NumberFunctionsTestSuite, Floor) {
    std::string code = R"(
        print(floor(3.7))
    )";
    std::string expected = "3";

    std::istringstream input(code);
    std::ostringstream output;
    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

// Test round
TEST(NumberFunctionsTestSuite, Round) {
    std::string code = R"(
        print(round(3.2))
        print(round(3.7))
    )";
    std::string expected = "34";

    std::istringstream input(code);
    std::ostringstream output;
    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

// Test square root
TEST(NumberFunctionsTestSuite, Sqrt) {
    std::string code = R"(
        print(sqrt(9))
    )";
    std::string expected = "3";

    std::istringstream input(code);
    std::ostringstream output;
    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

// Test rnd function mod 1 -> always 0
TEST(NumberFunctionsTestSuite, Rnd) {
    std::string code = R"(
        print(rnd(1))
    )";
    std::string expected = "0";

    std::istringstream input(code);
    std::ostringstream output;
    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

// Test parse_num
TEST(NumberFunctionsTestSuite, ParseNum) {
    std::string code = R"(
        print(parse_num("123.45"))
        print(parse_num("abc"))
    )";
    std::string expected = "123.45nil";

    std::istringstream input(code);
    std::ostringstream output;
    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

// Test to_string
TEST(NumberFunctionsTestSuite, ToString) {
    std::string code = R"(
        print(to_string(3.14))
    )";
    std::string expected = "3.14";

    std::istringstream input(code);
    std::ostringstream output;
    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}
