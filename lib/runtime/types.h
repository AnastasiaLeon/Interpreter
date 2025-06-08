// основа системы типов, обесп. хранение и манипуляция всеми возможными значениями в языке
#pragma once
#include <variant>
#include <memory>
#include <vector>
#include <string>
#include "parser/ast.h"
using NullType = std::monostate; // пустая структура 

// предварительное объявление структур для хранения значений
struct ListValue;
struct FunctionValue;

// псевдонимы для списка, функции и варианта Value
using List = std::shared_ptr<ListValue>;
using Function = std::shared_ptr<FunctionValue>;
using Value = std::variant<NullType, double, std::string, List, Function>;

// определение типов значений списка и функции
struct ListValue {
    std::vector<Value> elements;
};

struct FunctionValue {
    std::vector<std::string> parameters;
    std::vector<const ASTNode*> body;
};

struct ReturnException {
    Value value;
};

struct BreakException {};
struct ContinueException {};

extern std::vector<Value>* g_print_values;