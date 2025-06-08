#pragma once
#include "types.h"
#include "environment.h"
#include "parser/ast.h"
#include <vector>

Value evaluate(const ASTNode* node, Environment& env); // вычисление значения узла АСТ
void execNode(const ASTNode* node, Environment& env, std::vector<Value>& print_values); // выполнение узла АСТ 