#pragma once

#include <string>
#include <istream>
#include <cctype>
#include <sstream>
#include <iostream>
#include <cstdlib>

// типы токенов
enum class TokenType {
    NUMBER,
    STRING,
    IDENTIFIER,
    PRINT,
    TRUE,
    FALSE,
    NIL,
    FUNCTION,
    RETURN,
    IF,
    THEN,
    ELSE,
    END,
    FOR,
    IN,
    WHILE,
    BREAK,
    CONTINUE,
    
    // операторы
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    MODULO,
    POWER,
    
    // составные операторы присваивания
    EQUALS,
    PLUS_EQUALS,
    MINUS_EQUALS,
    MULTIPLY_EQUALS,
    DIVIDE_EQUALS,
    MODULO_EQUALS,
    POWER_EQUALS,
    
    // операторы сравнения
    EQUAL_EQUAL,
    NOT_EQUAL,
    LESS,
    GREATER,
    LESS_EQUAL,
    GREATER_EQUAL,
    
    // логические операторы
    AND,
    OR,
    NOT,
    
    // разделители
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACKET,
    RIGHT_BRACKET,
    COLON,
    SEMICOLON,
    COMMA,
    
    // специальные токены
    ERROR,
    END_OF_FILE
};

inline std::string token_type_to_string(TokenType type) {
    switch (type) {
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::PRINT: return "PRINT";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACKET: return "LEFT_BRACKET";
        case TokenType::RIGHT_BRACKET: return "RIGHT_BRACKET";
        case TokenType::COLON: return "COLON";
        case TokenType::PLUS: return "PLUS";
        case TokenType::MINUS: return "MINUS";
        case TokenType::MULTIPLY: return "MULTIPLY";
        case TokenType::DIVIDE: return "DIVIDE";
        case TokenType::MODULO: return "MODULO";
        case TokenType::POWER: return "POWER";
        case TokenType::EQUALS: return "EQUALS";
        case TokenType::PLUS_EQUALS: return "PLUS_EQUALS";
        case TokenType::MINUS_EQUALS: return "MINUS_EQUALS";
        case TokenType::MULTIPLY_EQUALS: return "MULTIPLY_EQUALS";
        case TokenType::DIVIDE_EQUALS: return "DIVIDE_EQUALS";
        case TokenType::MODULO_EQUALS: return "MODULO_EQUALS";
        case TokenType::POWER_EQUALS: return "POWER_EQUALS";
        case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
        case TokenType::NOT_EQUAL: return "NOT_EQUAL";
        case TokenType::LESS: return "LESS";
        case TokenType::GREATER: return "GREATER";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::AND: return "AND";
        case TokenType::OR: return "OR";
        case TokenType::NOT: return "NOT";
        case TokenType::TRUE: return "TRUE";
        case TokenType::FALSE: return "FALSE";
        case TokenType::NIL: return "NIL";
        case TokenType::STRING: return "STRING";
        case TokenType::IF: return "IF";
        case TokenType::THEN: return "THEN";
        case TokenType::ELSE: return "ELSE";
        case TokenType::END: return "END";
        case TokenType::FOR: return "FOR";
        case TokenType::IN: return "IN";
        case TokenType::WHILE: return "WHILE";
        case TokenType::BREAK: return "BREAK";
        case TokenType::CONTINUE: return "CONTINUE";
        case TokenType::FUNCTION: return "FUNCTION";
        case TokenType::RETURN: return "RETURN";
        case TokenType::ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}

struct Token {
    TokenType type;
    std::string value;
    double number_value = 0.0;
    std::string string_value;

    std::string to_string() const {
        std::stringstream ss;
        ss << "Token{type=" << token_type_to_string(type) << ", value='" << value << "'";
        if (type == TokenType::NUMBER) {
            ss << ", number=" << number_value;
        } else if (type == TokenType::STRING) {
            ss << ", string='" << string_value << "'";
        }
        ss << "}";
        return ss.str();
    }
};

class Lexer {
public:
    explicit Lexer(std::istream& input) : input_(input) {
        read_char();
    }

    Token next_token();
    Token current_token() const { return current_token_; }
    void expect(TokenType type);

private:
    std::istream& input_;
    char current_char_ = 0;
    Token current_token_;

    void read_char();
    char peek_next_char();
    void skip_whitespace();
    void skip_line_comment();
    Token read_number();
    std::string read_identifier();
    std::string read_string();
}; 