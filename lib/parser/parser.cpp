#include "parser.h"

Parser::Parser(Lexer& l) : lexer_(l) {
    current_token_ = lexer_.next_token();
}

std::vector<std::unique_ptr<ASTNode>> Parser::parse() {
    std::vector<std::unique_ptr<ASTNode>> statements; // собрать все statements в вектор указателей на узлы АСТ
    
    while (current_token_.type != TokenType::END_OF_FILE) {
        statements.push_back(parse_statement());
    }
    
    return statements;
}

void Parser::next_token() {
    current_token_ = lexer_.next_token();
}

std::unique_ptr<ASTNode> Parser::parse_statement() {
    // оператор return
    if (current_token_.type == TokenType::RETURN) {
        next_token();
        auto expr = parse_expression();
        return std::make_unique<ReturnNode>(std::move(expr));
    }
    // оператор print
    if (current_token_.type == TokenType::PRINT) {
        next_token();  // пропустить 'print'
        if (current_token_.type != TokenType::LEFT_PAREN) throw std::runtime_error("ожидалась '(' после print");
        next_token();  // пропустить '('
        auto expr = parse_expression();
        if (current_token_.type != TokenType::RIGHT_PAREN) throw std::runtime_error("ожидалась ')' после выражения print");
        next_token();  // пропустить ')'
        return std::make_unique<PrintNode>(std::move(expr));
    }
    // условный оператор if
    if (current_token_.type == TokenType::IF) {
        next_token();  // пропустить 'if'
        auto condition = parse_expression();
        if (current_token_.type != TokenType::THEN) throw std::runtime_error("ожидалось 'then' после условия if");
        next_token();  // пропустить 'then'
        std::vector<std::unique_ptr<ASTNode>> then_block;
        while (current_token_.type != TokenType::ELSE && current_token_.type != TokenType::END && current_token_.type != TokenType::END_OF_FILE) {
            then_block.push_back(parse_statement());
        }
        std::vector<std::pair<std::unique_ptr<ASTNode>, std::vector<std::unique_ptr<ASTNode>>>> branches;
        branches.emplace_back(std::move(condition), std::move(then_block));
        std::vector<std::unique_ptr<ASTNode>> else_block;
        // ветви else-if и else
        while (current_token_.type == TokenType::ELSE) {
            next_token();  // пропустить 'else'
            if (current_token_.type == TokenType::IF) {
                next_token();  // пропустить 'if'
                auto elif_cond = parse_expression();
                if (current_token_.type != TokenType::THEN) throw std::runtime_error("ожидалось 'then' после условия else if");
                next_token();  // пропустить 'then'
                std::vector<std::unique_ptr<ASTNode>> elif_block;
                while (current_token_.type != TokenType::ELSE && current_token_.type != TokenType::END && current_token_.type != TokenType::END_OF_FILE) {
                    elif_block.push_back(parse_statement());
                }
                branches.emplace_back(std::move(elif_cond), std::move(elif_block));
            } else {
                // ветвь else
                while (current_token_.type != TokenType::END && current_token_.type != TokenType::END_OF_FILE) {
                    else_block.push_back(parse_statement());
                }
                break;
            }
        }
        if (current_token_.type != TokenType::END) throw std::runtime_error("ожидалось 'end' для закрытия оператора if");
        next_token();  // пропустить 'end'
        if (current_token_.type != TokenType::IF) throw std::runtime_error("ожидалось 'if' после end");
        next_token();  // пропустить 'if'
        return std::make_unique<IfNode>(std::move(branches), std::move(else_block));
    }
    // цикл for
    if (current_token_.type == TokenType::FOR) {
        next_token();  // пропустить 'for'
        if (current_token_.type != TokenType::IDENTIFIER) throw std::runtime_error("ожидался идентификатор после for");
        std::string var_name = current_token_.value;
        next_token();  // пропустить идентификатор
        if (current_token_.type != TokenType::IN) throw std::runtime_error("ожидалось 'in' в цикле for");
        next_token();  // пропустить 'in'
        auto iterable = parse_expression();
        std::vector<std::unique_ptr<ASTNode>> body;
        while (current_token_.type != TokenType::END && current_token_.type != TokenType::END_OF_FILE) {
            body.push_back(parse_statement());
        }
        if (current_token_.type != TokenType::END) throw std::runtime_error("ожидалось 'end' для закрытия цикла for");
        next_token();  // пропустить 'end'
        if (current_token_.type != TokenType::FOR) throw std::runtime_error("ожидалось 'for' после end");
        next_token();  // пропустить 'for'
        return std::make_unique<ForNode>(std::move(var_name), std::move(iterable), std::move(body));
    }
    // цикл while
    if (current_token_.type == TokenType::WHILE) {
        next_token();  // пропустить 'while'
        auto condition = parse_expression();
        std::vector<std::unique_ptr<ASTNode>> body;
        while (current_token_.type != TokenType::END && current_token_.type != TokenType::END_OF_FILE) {
            body.push_back(parse_statement());
        }
        if (current_token_.type != TokenType::END) throw std::runtime_error("ожидалось 'end' для закрытия цикла while");
        next_token();  // пропустить 'end'
        if (current_token_.type != TokenType::WHILE) throw std::runtime_error("ожидалось 'while' после end");
        next_token();  // пропустить 'while'
        return std::make_unique<WhileNode>(std::move(condition), std::move(body));
    }
    // оператор break
    if (current_token_.type == TokenType::BREAK) {
        next_token();
        return std::make_unique<BreakNode>();
    }
    // оператор continue
    if (current_token_.type == TokenType::CONTINUE) {
        next_token();
        return std::make_unique<ContinueNode>();
    }
    // выражение или присваивание
    auto expr = parse_expression();
    return expr;
}

std::unique_ptr<ASTNode> Parser::parse_expression() {
    return parse_assignment();
}

std::unique_ptr<ASTNode> Parser::parse_assignment() {
    auto expr = parse_logical_or();
    
    if (current_token_.type == TokenType::EQUALS ||
        current_token_.type == TokenType::PLUS_EQUALS ||
        current_token_.type == TokenType::MINUS_EQUALS ||
        current_token_.type == TokenType::MULTIPLY_EQUALS ||
        current_token_.type == TokenType::DIVIDE_EQUALS ||
        current_token_.type == TokenType::MODULO_EQUALS ||
        current_token_.type == TokenType::POWER_EQUALS) {
        if (auto var = dynamic_cast<VariableNode*>(expr.get())) {
            auto op = current_token_.type;
            next_token();
            auto value = parse_assignment();
            return std::make_unique<AssignNode>(var->name, op, std::move(value));
        }
        throw std::runtime_error("недопустимая цель присваивания");
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::parse_logical_or() {
    auto expr = parse_logical_and();
    
    while (current_token_.type == TokenType::OR) {
        auto op = current_token_.type;
        next_token();
        auto right = parse_logical_and();
        expr = std::make_unique<BinaryOpNode>(op, std::move(expr), std::move(right));
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::parse_logical_and() {
    auto expr = parse_equality();
    
    while (current_token_.type == TokenType::AND) {
        auto op = current_token_.type;
        next_token();
        auto right = parse_equality();
        expr = std::make_unique<BinaryOpNode>(op, std::move(expr), std::move(right));
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::parse_equality() {
    auto expr = parse_comparison();
    
    while (current_token_.type == TokenType::EQUAL_EQUAL ||
           current_token_.type == TokenType::NOT_EQUAL) {
        auto op = current_token_.type;
        next_token();
        auto right = parse_comparison();
        expr = std::make_unique<BinaryOpNode>(op, std::move(expr), std::move(right));
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::parse_comparison() {
    auto expr = parse_term();
    
    while (current_token_.type == TokenType::LESS ||
           current_token_.type == TokenType::GREATER ||
           current_token_.type == TokenType::LESS_EQUAL ||
           current_token_.type == TokenType::GREATER_EQUAL) {
        auto op = current_token_.type;
        next_token();
        auto right = parse_term();
        expr = std::make_unique<BinaryOpNode>(op, std::move(expr), std::move(right));
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::parse_term() {
    auto expr = parse_factor();
    
    while (current_token_.type == TokenType::PLUS ||
           current_token_.type == TokenType::MINUS) {
        auto op = current_token_.type;
        next_token();
        auto right = parse_factor();
        expr = std::make_unique<BinaryOpNode>(op, std::move(expr), std::move(right));
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::parse_factor() {
    auto expr = parse_power();
    
    while (current_token_.type == TokenType::MULTIPLY ||
           current_token_.type == TokenType::DIVIDE ||
           current_token_.type == TokenType::MODULO) {
        auto op = current_token_.type;
        next_token();
        auto right = parse_power();
        expr = std::make_unique<BinaryOpNode>(op, std::move(expr), std::move(right));
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::parse_power() {
    auto expr = parse_unary();
    
    while (current_token_.type == TokenType::POWER) {
        auto op = current_token_.type;
        next_token();
        auto right = parse_unary();
        expr = std::make_unique<BinaryOpNode>(op, std::move(expr), std::move(right));
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::parse_unary() {
    if (current_token_.type == TokenType::PLUS ||
        current_token_.type == TokenType::MINUS ||
        current_token_.type == TokenType::NOT) {
        auto op = current_token_.type;
        next_token();
        auto operand = parse_unary();
        return std::make_unique<UnaryOpNode>(op, std::move(operand));
    }
    
    return parse_primary();
}

std::unique_ptr<ASTNode> Parser::parse_primary() {
    auto expr = parse_atom();
    
    // обработка вызовов функций
    while (current_token_.type == TokenType::LEFT_PAREN) {
        next_token();  // пропустить '('
        std::vector<std::unique_ptr<ASTNode>> args;
        if (current_token_.type != TokenType::RIGHT_PAREN) {
            while (true) {
                args.push_back(parse_expression());
                if (current_token_.type == TokenType::COMMA) next_token();
                else break;
            }
        }
        if (current_token_.type != TokenType::RIGHT_PAREN) throw std::runtime_error("ожидалась ')' после аргументов функции");
        next_token();  // пропустить ')'
        expr = std::make_unique<CallNode>(std::move(expr), std::move(args));
    }
    
    while (current_token_.type == TokenType::LEFT_BRACKET) {
        next_token();  // пропустить '['
        
        // обработка срезов
        if (current_token_.type == TokenType::COLON) {
            next_token();  // пропустить ':'
            std::unique_ptr<ASTNode> end;
            if (current_token_.type != TokenType::RIGHT_BRACKET) {
                end = parse_expression();
            }
            if (current_token_.type != TokenType::RIGHT_BRACKET) {
                throw std::runtime_error("ожидалась ']'");
            }
            next_token();  // пропустить ']'
            expr = std::make_unique<SliceNode>(std::move(expr), nullptr, std::move(end));
        } else {
            // обработка индексации или среза с начальным индексом
            auto start = parse_expression();
            
            if (current_token_.type == TokenType::COLON) {
                next_token();  // пропустить ':'
                std::unique_ptr<ASTNode> end;
                if (current_token_.type != TokenType::RIGHT_BRACKET) {
                    end = parse_expression();
                }
                if (current_token_.type != TokenType::RIGHT_BRACKET) {
                    throw std::runtime_error("ожидалась ']'");
                }
                next_token();  // пропустить ']'
                expr = std::make_unique<SliceNode>(std::move(expr), std::move(start), std::move(end));
            } else {
                if (current_token_.type != TokenType::RIGHT_BRACKET) {
                    throw std::runtime_error("ожидалась ']'");
                }
                next_token();  // пропустить ']'
                expr = std::make_unique<IndexNode>(std::move(expr), std::move(start));
            }
        }
    }
    
    // обработка вызовов функций после индексации/среза (например, funcs[0]())
    while (current_token_.type == TokenType::LEFT_PAREN) {
        next_token();  // пропустить '('
        std::vector<std::unique_ptr<ASTNode>> args;
        if (current_token_.type != TokenType::RIGHT_PAREN) {
            while (true) {
                args.push_back(parse_expression());
                if (current_token_.type == TokenType::COMMA) next_token();
                else break;
            }
        }
        if (current_token_.type != TokenType::RIGHT_PAREN) throw std::runtime_error("ожидалась ')' после аргументов функции");
        next_token();  // пропустить ')'
        expr = std::make_unique<CallNode>(std::move(expr), std::move(args));
    }
    
    return expr;
}

std::unique_ptr<ASTNode> Parser::parse_atom() {
    // функциональный литерал
    if (current_token_.type == TokenType::FUNCTION) {
        next_token(); // пропустить 'function'
        if (current_token_.type != TokenType::LEFT_PAREN) throw std::runtime_error("ожидалась '(' после function");
        next_token(); // пропустить '('
        std::vector<std::string> params;
        if (current_token_.type != TokenType::RIGHT_PAREN) {
            while (true) {
                if (current_token_.type != TokenType::IDENTIFIER) throw std::runtime_error("ожидалось имя параметра");
                params.push_back(current_token_.value);
                next_token();
                if (current_token_.type == TokenType::COMMA) next_token(); else break;
            }
        }
        if (current_token_.type != TokenType::RIGHT_PAREN) throw std::runtime_error("ожидалась ')' после параметров");
        next_token(); // пропустить ')'
        // разбор тела функции
        std::vector<std::unique_ptr<ASTNode>> body;
        while (current_token_.type != TokenType::END) {
            body.push_back(parse_statement());
        }
        next_token(); // пропустить 'end'
        if (current_token_.type != TokenType::FUNCTION) throw std::runtime_error("ожидалось 'function' после end");
        next_token(); // пропустить 'function'
        return std::make_unique<FunctionNode>(std::move(params), std::move(body));
    }
    if (current_token_.type == TokenType::NUMBER) {
        auto node = std::make_unique<NumberNode>(current_token_.number_value);
        next_token();
        return node;
    }
    
    if (current_token_.type == TokenType::STRING) {
        auto node = std::make_unique<StringNode>(current_token_.string_value);
        next_token();
        return node;
    }
    
    if (current_token_.type == TokenType::TRUE) {
        auto node = std::make_unique<NumberNode>(1.0);
        next_token();
        return node;
    }
    
    if (current_token_.type == TokenType::FALSE) {
        auto node = std::make_unique<NumberNode>(0.0);
        next_token();
        return node;
    }
    
    if (current_token_.type == TokenType::NIL) {
        next_token();
        return std::make_unique<NullNode>();
    }
    
    if (current_token_.type == TokenType::IDENTIFIER) {
        auto node = std::make_unique<VariableNode>(current_token_.value);
        next_token();
        return node;
    }
    
    if (current_token_.type == TokenType::LEFT_PAREN) {
        next_token();
        auto expr = parse_expression();
        if (current_token_.type != TokenType::RIGHT_PAREN) {
            throw std::runtime_error("ожидалась ')'");
        }
        next_token();
        return expr;
    }

    if (current_token_.type == TokenType::LEFT_BRACKET) {
        return parse_list();
    }

    throw std::runtime_error("ожидалось число, строка, идентификатор или '('");
}

std::unique_ptr<ASTNode> Parser::parse_list() {
    next_token();  // пропустить '['
    std::vector<std::unique_ptr<ASTNode>> elements;

    // разбор элементов, разрешая завершающую запятую
    while (current_token_.type != TokenType::RIGHT_BRACKET) {
        elements.push_back(parse_expression());
        if (current_token_.type == TokenType::COMMA) {
            next_token();  // пропустить запятую (разрешает завершающую запятую)
            continue;
        } else {
            break;
        }
    }

    if (current_token_.type != TokenType::RIGHT_BRACKET) {
        throw std::runtime_error("ожидалась ']'");
    }
    next_token();  // пропустить ']'
    return std::make_unique<ListNode>(std::move(elements));
}