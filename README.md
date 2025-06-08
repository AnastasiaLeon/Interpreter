# Interpreter
Interpreter for DataFlowScript

## Description

DataFlowScript is a lightweight, dynamically-typed interpreted programming language designed for simplicity and flexibility. The DataFlowScript interpreter processes source files with the .dfs extension, executing code line-by-line with robust error handling and automatic memory management. The language supports a variety of data types, operators, control structures, and a standard library, making it suitable for scripting and rapid prototyping. This project includes a fully implemented lexer, parser, abstract syntax tree (AST), and a comprehensive test suite using the Google Test framework.

## Features

### General Syntax

- Case Sensitivity: DataFlowScript is case-sensitive, ensuring precise identifier handling.
- Whitespace: Spaces and tabs are ignored, allowing flexible code formatting.
- Comments: Single-line comments are supported using //, extending to the end of the line.
- Expression Scope: Expressions are confined to a single line, ensuring clear and predictable parsing.

### Data Types

1.**Numbers**
- Signed, double-precision floating-point numbers (equivalent to C++ double).
- Special boolean literals: true (1) and false (0).
- Scientific notation support (e.g., 1.23e-4).

2.**Strings**
- String literals are enclosed in double quotes ("").
- Escape sequences are fully supported (e.g., "Some \"string\" type").

3.**Lists**
- Dynamic arrays defined with square brackets (e.g., [1, 2, 3]).
- Zero-based indexing with support for slicing.

4.**NullType**
- Represents the absence of a value with the literal nil.

5.**Functions**
- First-class objects, defined using the function keyword.
- Support for variable arguments and passing functions as arguments or return values.

### Operators

1. **Arithmetic**
  - `+`, `-`, `*`, `/`, `%` (modulus)
  - `^` (exponentiation)
  - unary`+` and `-`

2. **Comparison**
  - `==`, `!=`, `<`, `>`, `<=`, `>=`

3. **Logical**
  - `and`, `or`, `not`

4. **Assignment**
  - `=`, `+=`, `-=`, `*=`, `/=`, `%=`, `^=`

5. **Indexing/Slicing:**
  - `[]` (for accessing elements or slices in strings and lists)

  ### Operator Behavior

1. Numbers: Standard arithmetic, comparison, logical, and assignment operations.

2. Strings:
   - Comparison operators use lexicographical order.
   - + concatenates strings.
   - - removes a suffix if the first string ends with the second.
   - * repeats a string a specified number of times (not necessarily integer).
   - [] supports single-character access and slicing (e.g., s[n], s[:n], s[m:n], s[:]).

3. Lists:
   - + concatenates lists.
   - * repeats a list.
   - [] supports indexing and slicing, similar to strings.


4. NullType
   - Comparable with == (returns false except for nil) and != (inverse result).

### Control Structures

1. **Conditional Statements:**

  ```
    if condition then
        // statements
    else if condition then
        // statements
    else
        // statements
    end if
  ```


2. **Loops**

- `while`:
   ```
    while condition
        // statements
    end while
   ```


- `for`:
   ```
    for i in sequence
        // statements
    end for
   ```

3. **Loop Control**

 - break and continue function as expected within loops.

### Functions

- Defined using the function keyword and assigned to variables:

```
name = function(parameters)
    // statements
    return result
end function
```

- Functions are called with comma-separated arguments: name(arg1, arg2).

- Functions can be passed as arguments, returned, or aliased:

```
ilovefunctions("pass existing function...", func)
ilovefunctions("...or define one inplace", function(args)
    // do something
end function)
alias = anotherfunc
```

- Functions can define inner functions, but closures are not supported (inner functions do not capture outer variables).

### Scoping

- Global Scope: Variables and functions declared globally are accessible throughout the program.

- Local Scope: Includes function arguments and locally declared variables.

- Variables from one function’s scope can only be accessed in another via arguments.

- Shadowing of global variables and function arguments follows C++-style lexical scoping.

## Standart library

### Numeric Functions

- `abs(x)` - Absolute value.
- `ceil(x)` - Rounds up.
- `floor(x)` - Rounds down.
- `round(x)` - Rounds to the nearest integer.
- `sqrt(x)` - Square root.
- `rnd(n)` - Random integer from 0 to n-1.
- `parse_num(s)`  - Converts a string to a number, returns `nil` if invalid.
- `to_string(n)` - Converts a number to a string.

### String Functions

- `len(s)` - String length.
- `lower(s)` - Converts to lowercase.
- `upper(s)` - Converts to uppercase.
- `split(s, delim)` - Splits a string by delimiter.
- `join(list, delim)` -  Joins a list into a string with a delimiter.
- `replace(s, old, new)` - Replaces substrings.

### List Functions

- `range(x, y, step)` - Generates a list of numbers from x to y (exclusive) with step.
- `len(list)` - List length.
- `push(list, x)` - Appends an element.
- `pop(list)` - Removes and returns the last element.
- `insert(list, index, x)` - Inserts an element at the index.
- `remove(list, index)` - Removes an element at the index.
- `sort(list)` -  Sorts the list (behavior for mixed types is implementation-defined).

### System Functions

- `print(x)` - Outputs to the output stream without newlines.
- `println(x)` - Outputs with a trailing newline.
- `read()` - Reads a string from the input stream.
- `stacktrace()` - Returns the current function call stack (format implementation-defined).

## Implementation Details

1. **Dynamic Typing** - Types are checked at runtime.
2. **Memory Management** - Automatic garbage collection ensures variables are cleaned up when out of scope.
3. **Lexical Scoping** - Variables are visible in their declaration block, with C++-style shadowing.
4. **Interpretation** - Code is executed line-by-line, with syntax errors detected during execution.
5. **Safety** - Invalid operations are caught and handled gracefully without crashing the interpreter.
6. **Value Semantics**
- Simple types (numbers, nil) are copied by value.
- Complex types (strings, lists, functions) are passed by reference, similar to Python.

## Tests

The interpreter is thoroughly tested using the Google Test framework, covering all language features, including data types, operators, control structures, functions, and the standard library. The test suite ensures robust error handling and correct behavior across edge cases.

## Usage

DataFlowScript source files use the .dfs extension. The interpreter processes these files, executing the code and handling output via provided streams.

## Design

The interpreter is built with a modular architecture:
- **Lexer**: Tokenizes the input source code.
- **Parser**: Constructs an abstract syntax tree (AST) from tokens.
- **AST**: Represents the program structure for evaluation.
- **Evaluator**: Executes the AST, handling dynamic typing and runtime checks.

The implementation draws inspiration from resources like the LLVM Tutorial (Chapters 1 and 2), Let’s Build A Simple Interpreter, BNF, and Writing An Interpreter In Go, adapting their principles to C++ for a robust and extensible design.

## Examples

The directory `example` contains a number of example programs written in DataFlowScript.

 - fibonacci.is
 - maximum.is
 - fuzzBuzz.is

## Help with using

To explore DataFlowScript:
1. Clone the reposritory.
2. Build the project using a C++20-compliant compiler.
3. Run the interpreter with a .dfs file or use the provided stream-based interface.
4. Run the test suite to verify functionality.