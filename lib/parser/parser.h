#pragma once
#include "ast.h"
#include "lexer/lexer.h"
#include <vector>
#include <stdexcept>

class Parser {
public:
    explicit Parser(Lexer& l);
    std::vector<std::unique_ptr<ASTNode>> parse();

private:
    Lexer& lexer_;
    Token current_token_;

    void next_token();
    std::unique_ptr<ASTNode> parse_statement();
    std::unique_ptr<ASTNode> parse_expression();
    std::unique_ptr<ASTNode> parse_assignment();
    std::unique_ptr<ASTNode> parse_logical_or();
    std::unique_ptr<ASTNode> parse_logical_and();
    std::unique_ptr<ASTNode> parse_equality();
    std::unique_ptr<ASTNode> parse_comparison();
    std::unique_ptr<ASTNode> parse_term();
    std::unique_ptr<ASTNode> parse_factor();
    std::unique_ptr<ASTNode> parse_power();
    std::unique_ptr<ASTNode> parse_unary();
    std::unique_ptr<ASTNode> parse_primary();
    std::unique_ptr<ASTNode> parse_atom();
    std::unique_ptr<ASTNode> parse_list();
};