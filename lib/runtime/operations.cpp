#include "operations.h"
#include "utils.h"
#include <cmath>
#include <stdexcept>

Value apply_binary_op(const Value& left, const Value& right, TokenType op) {
    if (std::holds_alternative<NullType>(left) || std::holds_alternative<NullType>(right)) {
        if (op == TokenType::EQUAL_EQUAL) {
            return static_cast<double>(std::holds_alternative<NullType>(left) && std::holds_alternative<NullType>(right));
        } else if (op == TokenType::NOT_EQUAL) {
            return static_cast<double>(!(std::holds_alternative<NullType>(left) && std::holds_alternative<NullType>(right)));
        }
        throw std::runtime_error("Недопустимые операнды для бинарного оператора");
    }
    if (std::holds_alternative<List>(left)) {
        const List& list_left = std::get<List>(left);
        if (op == TokenType::PLUS && std::holds_alternative<List>(right)) {
            const List& list_right = std::get<List>(right);
            auto result = std::make_shared<ListValue>();
            result->elements = list_left->elements;
            result->elements.insert(result->elements.end(), list_right->elements.begin(), list_right->elements.end());
            return result;
        } else if (op == TokenType::MULTIPLY && std::holds_alternative<double>(right)) {
            double count = std::get<double>(right);
            if (count <= 0) return std::make_shared<ListValue>();
            auto result = std::make_shared<ListValue>();
            int full_repeats = static_cast<int>(count);
            for (int i = 0; i < full_repeats; ++i) {
                result->elements.insert(result->elements.end(), list_left->elements.begin(), list_left->elements.end());
            }
            return result;
        }
    }
    if (std::holds_alternative<std::string>(left)) {
        const std::string& str_left = std::get<std::string>(left);
        if (op == TokenType::PLUS) {
            if (std::holds_alternative<std::string>(right)) return str_left + std::get<std::string>(right);
            if (std::holds_alternative<double>(right)) return str_left + format_number(std::get<double>(right));
        } else if (op == TokenType::MINUS && std::holds_alternative<std::string>(right)) {
            const std::string& str_right = std::get<std::string>(right);
            if (str_left.size() >= str_right.size() && str_left.substr(str_left.size() - str_right.size()) == str_right) {
                return str_left.substr(0, str_left.size() - str_right.size());
            }
            return str_left;
        } else if (op == TokenType::MULTIPLY && std::holds_alternative<double>(right)) {
            double count = std::get<double>(right);
            if (count <= 0) return std::string();
            std::string result;
            int full_repeats = static_cast<int>(count);
            for (int i = 0; i < full_repeats; ++i) result += str_left;
            double fraction = count - full_repeats;
            if (fraction > 0) {
                int chars_to_add = static_cast<int>(str_left.size() * fraction);
                result += str_left.substr(0, chars_to_add);
            }
            return result;
        } else if (std::holds_alternative<std::string>(right)) {
            const std::string& str_right = std::get<std::string>(right);
            switch (op) {
                case TokenType::EQUAL_EQUAL: return static_cast<double>(str_left == str_right);
                case TokenType::NOT_EQUAL: return static_cast<double>(str_left != str_right);
                case TokenType::LESS: return static_cast<double>(str_left < str_right);
                case TokenType::LESS_EQUAL: return static_cast<double>(str_left <= str_right);
                case TokenType::GREATER: return static_cast<double>(str_left > str_right);
                case TokenType::GREATER_EQUAL: return static_cast<double>(str_left >= str_right);
                default: break;
            }
        }
    }
    if (std::holds_alternative<double>(left) && std::holds_alternative<double>(right)) {
        double l = std::get<double>(left);
        double r = std::get<double>(right);
        switch (op) {
            case TokenType::PLUS: return l + r;
            case TokenType::MINUS: return l - r;
            case TokenType::MULTIPLY: return l * r;
            case TokenType::DIVIDE:
                if (r == 0) throw std::runtime_error("Деление на ноль");
                return l / r;
            case TokenType::MODULO:
                if (r == 0) throw std::runtime_error("Модуль по нулю");
                return std::fmod(l, r);
            case TokenType::POWER: return std::pow(l, r);
            case TokenType::EQUAL_EQUAL: return static_cast<double>(l == r);
            case TokenType::NOT_EQUAL: return static_cast<double>(l != r);
            case TokenType::LESS: return static_cast<double>(l < r);
            case TokenType::LESS_EQUAL: return static_cast<double>(l <= r);
            case TokenType::GREATER: return static_cast<double>(l > r);
            case TokenType::GREATER_EQUAL: return static_cast<double>(l >= r);
            case TokenType::AND: return static_cast<double>((l != 0) && (r != 0));
            case TokenType::OR: return static_cast<double>((l != 0) || (r != 0));
            default: break;
        }
    }
    throw std::runtime_error("Недопустимые операнды для бинарного оператора");
}

Value apply_unary_op(TokenType op, const Value& operand) {
    if (!std::holds_alternative<double>(operand)) {
        throw std::runtime_error("Унарные операторы могут применяться только к числам");
    }
    double num = std::get<double>(operand);
    switch (op) {
        case TokenType::PLUS: return num;
        case TokenType::MINUS: return -num;
        case TokenType::NOT: return num == 0.0 ? 1.0 : 0.0;
        default: throw std::runtime_error("Неизвестный унарный оператор");
    }
}

TokenType get_binary_op_from_compound_assign(TokenType op) {
    switch (op) {
        case TokenType::PLUS_EQUALS: return TokenType::PLUS;
        case TokenType::MINUS_EQUALS: return TokenType::MINUS;
        case TokenType::MULTIPLY_EQUALS: return TokenType::MULTIPLY;
        case TokenType::DIVIDE_EQUALS: return TokenType::DIVIDE;
        case TokenType::MODULO_EQUALS: return TokenType::MODULO;
        case TokenType::POWER_EQUALS: return TokenType::POWER;
        default: throw std::runtime_error("Недопустимый оператор составного присваивания");
    }
}