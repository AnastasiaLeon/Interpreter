#include <lib/interpreter.h>
#include <gtest/gtest.h>
#include "../lib/lexer/lexer.h"
#include "../lib/parser/parser.h"
#include <sstream>


TEST(TypesTestSuite, IntTest) {
    std::string code = R"(
        x = 1
        y = 2
        z = 3 * x + y
        print(z)
    )";

    std::string expected = "5";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(StringTestSuite, BasicStringOperations) {
    std::string code = R"(
        s1 = "Hello"
        s2 = " World"
        s3 = s1 + s2  // Concatenation
        print(s3)
        s4 = s3 - "World"  // Suffix removal
        print(s4)
        s5 = "Ha" * 3  // Repetition
        print(s5)
    )";

    std::string expected = "Hello WorldHello HaHaHa";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(StringTestSuite, StringComparisons) {
    std::string code = R"(
        print("abc" < "def")  // true (1)
        print("abc" > "def")  // false (0)
        print("abc" == "abc")  // true (1)
        print("abc" != "def")  // true (1)
        print("abc" <= "abc")  // true (1)
        print("def" >= "abc")  // true (1)
    )";

    std::string expected = "101111";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(StringTestSuite, StringIndexing) {
    std::string code = R"(
        s = "Hello World"
        print(s[0])   // "H"
        print(s[6])   // "W"
        print(s[-1])  // "d"
        print(s[-6])  // " "
    )";

    std::string expected = "HWd ";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(StringTestSuite, StringSlicing) {
    std::string code = R"(
        s = "Hello World"
        print(s[0:5])    // "Hello"
        print(s[6:])     // "World"
        print(s[:5])     // "Hello"
        print(s[:])      // "Hello World"
        print(s[-5:])    // "World"
        print(s[:-6])    // "Hello"
    )";

    std::string expected = "HelloWorldHelloHello WorldWorldHello";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(StringTestSuite, EscapeSequences) {
    std::string code = R"(
        s = "Hello\nWorld\tTab\"Quote\\"
        print(s)
    )";

    std::string expected = "Hello\nWorld\tTab\"Quote\\";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(ListTestSuite, BasicListOperations) {
    std::string code = R"(
        l1 = [1, 2, 3]
        l2 = [4, 5, 6]
        l3 = l1 + l2  // Concatenation
        print(l3)
        l4 = [1, 2] * 3  // Repetition
        print(l4)
    )";

    std::string expected = "[1, 2, 3, 4, 5, 6][1, 2, 1, 2, 1, 2]";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(ListTestSuite, ListIndexing) {
    std::string code = R"(
        l = [10, 20, 30, 40, 50]
        print(l[0])   // 10
        print(l[2])   // 30
        print(l[-1])  // 50
        print(l[-2])  // 40
    )";

    std::string expected = "10305040";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(ListTestSuite, ListSlicing) {
    std::string code = R"(
        l = [1, 2, 3, 4, 5]
        print(l[0:3])    // [1, 2, 3]
        print(l[2:])     // [3, 4, 5]
        print(l[:2])     // [1, 2]
        print(l[:])      // [1, 2, 3, 4, 5]
        print(l[-3:])    // [3, 4, 5]
        print(l[:-2])    // [1, 2, 3]
    )";

    std::string expected = "[1, 2, 3][3, 4, 5][1, 2][1, 2, 3, 4, 5][3, 4, 5][1, 2, 3]";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(ListTestSuite, MixedTypes) {
    std::string code = R"(
        l = [1, "hello", 3.14, "world"]
        print(l)
        print(l[1])  // "hello"
        print(l[2])  // 3.14
    )";

    std::string expected = R"([1, "hello", 3.14, "world"]hello3.14)";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(NilTest, LexerRecognizesNil) {
    std::istringstream input("nil");
    Lexer lexer(input);
    Token token = lexer.next_token();
    EXPECT_EQ(token.type, TokenType::NIL);
    EXPECT_EQ(token.value, "nil");
}

TEST(NilTest, ParserCreatesNullNode) {
    std::istringstream input("nil");
    Lexer lexer(input);
    Parser parser(lexer);
    auto ast = parser.parse();
    ASSERT_EQ(ast.size(), 1);
    EXPECT_NE(dynamic_cast<NullNode*>(ast[0].get()), nullptr);
}

TEST(NilTest, InterpreterEvaluatesNil) {
    std::istringstream input("print(nil)");
    std::ostringstream output;
    interpret(input, output);
    EXPECT_EQ(output.str(), "nil");
}

TEST(NilTest, NilComparison) {
    std::string code = R"(
        print(nil == nil)
        print(nil != nil)
        print(nil == 0)
        print(nil != 0)
    )";
    std::istringstream input(code);
    std::ostringstream output;
    ASSERT_TRUE(interpret(input, output));
    EXPECT_EQ(output.str(), "1001");
}

TEST(NilTest, PrintNil) {
    std::istringstream input("print(nil)");
    std::ostringstream output;
    interpret(input, output);
    EXPECT_EQ(output.str(), "nil");
}

TEST(NumberTestSuite, ArithmeticOperations) {
    std::string code = R"(
        x = 5.5
        y = 2.0
        z = 1.23e-1  // 0.123
        print(x + y)  // 7.5
        print(x - y)  // 3.5
        print(x * y)  // 11.0
        print(x / y)  // 2.75
        print(x % y)  // 1.5
        print(x ^ y)  // 30.25
        print(-x)     // -5.5
        print(+x)     // 5.5
        print(z * 100) // 12.3
    )";

    std::string expected = "7.53.5112.751.530.25-5.55.512.3";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(LogicTestSuite, LogicalOperations) {
    std::string code = R"(
        a = true
        b = false
        c = 1
        d = 0
        print(a and b)    // false (0)
        print(a or b)     // true (1)
        print(not a)      // false (0)
        print(not b)      // true (1)
        print(c and a)    // true (1)
        print(d or b)     // false (0)
        print(not c)      // false (0)
        print(not d)      // true (1)
    )";

    std::string expected = "01011001";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(AssignmentTestSuite, CompoundAssignments) {
    std::string code = R"(
        x = 10.0
        x += 5.0
        print(x)  // 15.0
        x -= 3.0
        print(x)  // 12.0
        x *= 2.0
        print(x)  // 24.0
        x /= 4.0
        print(x)  // 6.0
        x %= 5.0
        print(x)  // 1.0
        x ^= 2.0
        print(x)  // 1.0
    )";

    std::string expected = "151224611";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}


//!!!!!!!!! умн на нецелое число строку

TEST(StringTestSuite, StringMultiplicationFloat) {
    std::string code = R"(
        s = "abc"
        print(s * 2.7)  // "abcabc" (2.7 округляется до 2)
        print(s * 0.9)  // "" (0.9 округляется до 0)
    )";

    std::string expected = "abcabcabab";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(StringTestSuite, StringSubtractionInvalidSuffix) {
    std::string code = R"(
        s = "Hello World"
        print(s - "xyz")  // "Hello World" (не суффикс, возвращается исходная строка)
    )";

    std::string expected = "Hello World";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(NumberTestSuite, ScientificNotation) {
    std::string code = R"(
        x = 1.23e-4
        y = 5e2
        print(x * 10000)  // 1.23
        print(y / 100)    // 5.0
    )";

    std::string expected = "1.235";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(CommentTestSuite, IgnoreComments) {
    std::string code = R"(
        x = 1 // This is a comment
        // Another comment
        y = 2
        print(x + y) // Print sum
    )";

    std::string expected = "3";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

/////



TEST(ListTestSuite, NestedLists) {
    std::string code = R"(
        l = [1, [2, 3, 4], 5]
        print(l)        // [1, [2, 3, 4], 5]
        print(l[1])     // [2, 3, 4]
        print(l[1][1])  // 3
    )";

    std::string expected = "[1, [2, 3, 4], 5][2, 3, 4]3";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(ErrorTestSuite, OutOfBoundsIndexing) {
    std::string code = R"(
        s = "abc"
        l = [1, 2, 3]
        print(s[10])  // Ожидается ошибка или nil
        print(l[5])   // Ожидается ошибка или nil
    )";

    std::string expected = "nilnil";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(MixedTestSuite, MixedOperations) {
    std::string code = R"(
        n = 42
        s = "test"
        l = [1, 2, 3]
        print(n + 8)         // 50
        print(s + "ing")     // "testing"
        print(l + [4, 5])    // [1, 2, 3, 4, 5]
        print(nil == l[10])  // true (out of bounds -> nil)
    )";

    std::string expected = "50testing[1, 2, 3, 4, 5]1";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(StringTestSuite, AdvancedEscapeSequences) {
    std::string code = R"(
        s = "Line1\nLine2\rTab\tQuote\"Backslash\\"
        print(s)
    )";

    std::string expected = "Line1\nLine2\rTab\tQuote\"Backslash\\";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}


TEST(EmptyTestSuite, EmptyStringAndList) {
    std::string code = R"(
        s = ""
        l = []
        print(s + "test")  // "test"
        print(l + [1])     // [1]
        print(s[0])        // nil
        print(l[0])        // nil
    )";

    std::string expected = "test[1]nilnil";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

