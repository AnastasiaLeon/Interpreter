#include <lib/interpreter.h>
#include <gtest/gtest.h>

TEST(FunctionTestSuite, SimpleFunctionTest) {
    std::string code = R"(
        incr = function(value)
            return value + 1
        end function

        x = incr(2)
        print(x)
    )";

    std::string expected = "3";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}


TEST(FunctionTestSuite, FunctionAsArgTest) {
    std::string code = R"(
        incr = function(value)
            return value + 1
        end function

        printresult = function(value, func)
            result = func(value)
            print(result)
        end function

        printresult(2, incr)
    )";

    std::string expected = "3";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}


TEST(FunctionTestSuite, NestedFunctionTest) {
    std::string code = R"(
        // NB: inner and outer `value` are different symbols.
        // You are not required to implement closures (aka lambdas).

        incr_and_print = function(value)
            incr = function(value)
                return value + 1
            end function

            print(incr(value))
        end function

        incr_and_print(2)
    )";

    std::string expected = "3";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}


TEST(FunctionTestSuite, FunnySyntaxTest) {
    std::string code = R"(
        funcs = [
            function() return 1 end function,
            function() return 2 end function,
            function() return 3 end function,
        ]

        print(funcs[0]())
        print(funcs[1]())
        print(funcs[2]())
    )";

    std::string expected = "123";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}


TEST(FunctionTestSuite, Fibonacci) {
    std::string code = R"(
        fib = function(n)
            if n == 0 then
                return 0
            end if
            a = 0
            b = 1
            for i in range(n - 1)
                c = a + b
                a = b
                b = c
            end for
            return b
        end function
        print(fib(10))
    )";

    std::string expected = "55";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(FunctionTestSuite, FizzBuzz) {
    std::string code = R"(
        fizzBuzz = function(n)
            for i in range(1, n)
                s = "Fizz" * (i % 3 == 0) + "Buzz" * (i % 5 == 0)
                if s == "" then
                    print(i)
                else
                    print(s)
                end if
            end for
        end function
        fizzBuzz(16)
    )";

    std::string expected = "12Fizz4BuzzFizz78FizzBuzz11Fizz1314FizzBuzz";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(FunctionTestSuite, Max) {
    std::string code = R"(
        max = function(arr)
            if len(arr) == 0 then
                return nil
            end if
            m = arr[0]
            for i in arr
                if i > m then m = i end if
            end for
            return m
        end function
        print(max([10, -1, 0, 2, 2025, 239]))
        print(max([]))
    )";

    std::string expected = "2025nil";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(FunctionTestSuite, ForLoopWithFunction) {
    std::string code = R"(
        square = function(x)
            return x * x
        end function
        for i in range(1, 5)
            print(square(i))
        end for
    )";

    std::string expected = "14916";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(FunctionTestSuite, MultipleArguments) {
    std::string code = R"(
        process = function(num, str, lst, nilval)
            print(num + 5)
            print(str + "!")
            print(lst[0])
            print(nilval == nil)
        end function
        process(10, "Hello", [1, 2, 3], nil)
    )";

    std::string expected = "15Hello!11";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}

TEST(FunctionTestSuite, FunctionAsArgument) {
    std::string code = R"(
        apply = function(f, x)
            return f(x)
        end function
        double = function(x)
            return x * 2
        end function
        print(apply(double, 5))
    )";

    std::string expected = "10";

    std::istringstream input(code);
    std::ostringstream output;

    ASSERT_TRUE(interpret(input, output));
    ASSERT_EQ(output.str(), expected);
}