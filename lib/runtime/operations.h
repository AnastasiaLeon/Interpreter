#pragma once
#include "types.h"
#include "lexer/lexer.h"

Value apply_binary_op(const Value& left, const Value& right, TokenType op);
Value apply_unary_op(TokenType op, const Value& operand);
TokenType get_binary_op_from_compound_assign(TokenType op);