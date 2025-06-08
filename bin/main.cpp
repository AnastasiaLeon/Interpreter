#include "../lib/interpreter.h"
#include <sstream>
#include <iostream>

void runTest(const std::string& name, const std::string& code) {
    // std::cout << "\n=== " << name << " ===" << std::endl;
    // std::cout << "Code:\n" << code << std::endl;
    std::istringstream input(code);
    std::ostringstream output;
    bool success = interpret(input, output);
    std::cout << "Success: " << (success ? "true" : "false") << std::endl;
    std::cout << "Output: " << output.str() << std::endl;
}

int main() {
        runTest("check", R"(
        a = "InTerPretER"
        b = lower(a)
        print(b)
    )");

    // runTest("Arithmetic and scientific notation", R"(
    //     a = 1.23e-4
    //     b = 4.56E2
    //     c = a + b
    //     d = b - a
    //     e = a * b
    //     f = b / a
    //     g = 10 % 3
    //     h = 2 ^ 3
    //     print(a)  // 0.000123
    //     print(b)  // 456
    //     print(c)  // ~456.000123
    //     print(d)  // ~455.999877
    //     print(e)  // ~0.056088
    //     print(f)  // ~3.707317e6
    //     print(g)  // 1
    //     print(h)  // 8
    // )");

    // runTest("Comparison operators", R"(
    //     print(5 == 5)    // 1
    //     print(5 != 3)    // 1
    //     print(5 > 3)     // 1
    //     print(5 < 3)     // 0
    //     print(5 >= 5)    // 1
    //     print(3 <= 5)    // 1
    // )");

    // runTest("Logical operators", R"(
    //     print(true and true)    // 1
    //     print(true and false)   // 0
    //     print(false or true)    // 1
    //     print(false or false)   // 0
    //     print(not false)        // 1
    //     print(not true)         // 0
    //     print(1 and 1)         // 1
    //     print(1 or 0)          // 1
    //     print(not 0)           // 1
    // )");

    // runTest("Complex expressions", R"(
    //     a = 2 + 3 * 4         // 14
    //     b = (2 + 3) * 4       // 20
    //     c = 2 ^ 3 + 4         // 12
    //     d = 2 ^ (3 + 4)       // 128
    //     e = 10 > 5 and 5 < 7  // 1
    //     f = not (10 < 5)      // 1
    //     print(a)
    //     print(b)
    //     print(c)
    //     print(d)
    //     print(e)
    //     print(f)
    // )");

    return 0;
}