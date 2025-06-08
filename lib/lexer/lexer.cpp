#include "lexer/lexer.h"
#include <cctype>
#include <stdexcept>
#include <sstream>

void Lexer::read_char() { // чтение 1 символа из входного потка
    current_char_ = input_.get();
}

char Lexer::peek_next_char() { // возврат след символа без извлечения
    char next = input_.peek();
    return next;
}

void Lexer::skip_whitespace() { // проверка пробельных симв (пробел, \t, \n и др.)
    while (std::isspace(current_char_)) {
        read_char();
    }
}

void Lexer::skip_line_comment() { // читаем до конца строки/файла/потока данных
    while (current_char_ != '\n' && current_char_ != EOF) {
        read_char();
    }
    if (current_char_ != EOF) {
        read_char();
    }
}

std::string Lexer::read_string() {
    std::string result;
    read_char();
    
    while (current_char_ != '"' && current_char_ != EOF) {
        if (current_char_ == '\\') { // escape-последовательность
            read_char();
            switch (current_char_) {
                case 'n': result += '\n'; break;
                case 'r': result += '\r'; break;
                case 't': result += '\t'; break;
                case '"': result += '"'; break;
                case '\\': result += '\\'; break;
                case 'b': result += '\b'; break;
                case 'f': result += '\f'; break;
                case 'v': result += '\v'; break;
                case 'a': result += '\a'; break;
                case '0': result += '\0'; break;
                default: result += '\\'; result += current_char_; break;
            }
        } else {
            result += current_char_;
        }
        read_char();
    }
    
    if (current_char_ == EOF) {
        throw std::runtime_error("Прерванный строковый литерал");
    }
    
    read_char();
    return result;
}

std::string Lexer::read_identifier() {
    std::string result;
    while (std::isalnum(current_char_) || current_char_ == '_') {
        result += current_char_;
        read_char();
    }
    return result;
}

Token Lexer::read_number() {
    std::string number_str;
    if (current_char_ == '-') {
        number_str += current_char_;
        read_char();
    }
    while (std::isdigit(current_char_)) {
        number_str += current_char_;
        read_char();
    }
    if (current_char_ == '.') {
        number_str += current_char_;
        read_char();
        while (std::isdigit(current_char_)) {
            number_str += current_char_;
            read_char();
        }
    }
    if (current_char_ == 'e' || current_char_ == 'E') {
        number_str += current_char_;
        read_char();
        if (current_char_ == '+' || current_char_ == '-') {
            number_str += current_char_;
            read_char();
        }
        if (!std::isdigit(current_char_)) {
            throw std::runtime_error("Невалидная экспонента в числовом литерале: " + number_str);
        }
        while (std::isdigit(current_char_)) {
            number_str += current_char_;
            read_char();
        }
    }
    
    try {
        double value = std::stod(number_str);
        return Token{TokenType::NUMBER, number_str, value};
    } catch (const std::exception& e) {
        throw std::runtime_error("Невалидный формат числа: " + number_str);
    }
}

void Lexer::expect(TokenType type) { // проверка соотв текущего токена ожидаемому типу
    if (current_token_.type != type) {
        throw std::runtime_error("Ожидаемый тип токена " + std::to_string(static_cast<int>(type)) + " а полученный " + std::to_string(static_cast<int>(current_token_.type)));
    }
    next_token();
}

Token Lexer::next_token() { // осн функция: читает вход поток, опред каждый токен, возвращает структуру Token
    skip_whitespace(); // пропуск пробелов и комментариев
    
    if (current_char_ == EOF) { // проверка на конец файла
        current_token_ = Token{TokenType::END_OF_FILE, ""};
        return current_token_;
    }
    
    if (current_char_ == '"') { // обработка строк
        std::string str_value = read_string();
        current_token_ = Token{TokenType::STRING, "\"" + str_value + "\"", 0.0, str_value};
        return current_token_;
    }
    
    if (std::isdigit(current_char_) || current_char_ == '.' || current_char_ == '-') { // обработка чисел
        if (current_char_ == '-') {
            char next = peek_next_char();
            if (!std::isdigit(next) && next != '.') {
                read_char();
                if (current_char_ == '=') {
                    read_char();
                    current_token_ = Token{TokenType::MINUS_EQUALS, "-="};
                    return current_token_;
                }
                current_token_ = Token{TokenType::MINUS, "-"};
                return current_token_;
            }
        }
        current_token_ = read_number();
        return current_token_;
    }
    
    if (std::isalpha(current_char_)) { // обработка идентификаторов и ключевых слов
        std::string identifier = read_identifier();
        if (identifier == "print") current_token_ = Token{TokenType::PRINT, identifier};
        else if (identifier == "true") current_token_ = Token{TokenType::TRUE, identifier, 1.0};
        else if (identifier == "false") current_token_ = Token{TokenType::FALSE, identifier, 0.0};
        else if (identifier == "nil") current_token_ = Token{TokenType::NIL, identifier};
        else if (identifier == "function") current_token_ = Token{TokenType::FUNCTION, identifier};
        else if (identifier == "return") current_token_ = Token{TokenType::RETURN, identifier};
        else if (identifier == "if") current_token_ = Token{TokenType::IF, identifier};
        else if (identifier == "then") current_token_ = Token{TokenType::THEN, identifier};
        else if (identifier == "else") current_token_ = Token{TokenType::ELSE, identifier};
        else if (identifier == "end") current_token_ = Token{TokenType::END, identifier};
        else if (identifier == "for") current_token_ = Token{TokenType::FOR, identifier};
        else if (identifier == "in") current_token_ = Token{TokenType::IN, identifier};
        else if (identifier == "while") current_token_ = Token{TokenType::WHILE, identifier};
        else if (identifier == "break") current_token_ = Token{TokenType::BREAK, identifier};
        else if (identifier == "continue") current_token_ = Token{TokenType::CONTINUE, identifier};
        else if (identifier == "and") current_token_ = Token{TokenType::AND, identifier};
        else if (identifier == "or") current_token_ = Token{TokenType::OR, identifier};
        else if (identifier == "not") current_token_ = Token{TokenType::NOT, identifier};
        else current_token_ = Token{TokenType::IDENTIFIER, identifier};
        return current_token_;
    }
    
    char c = current_char_;
    read_char();
    
    switch (c) { // обработка операторов и символов
        case '(': current_token_ = Token{TokenType::LEFT_PAREN, "("}; break;
        case ')': current_token_ = Token{TokenType::RIGHT_PAREN, ")"}; break;
        case '[': current_token_ = Token{TokenType::LEFT_BRACKET, "["}; break;
        case ']': current_token_ = Token{TokenType::RIGHT_BRACKET, "]"}; break;
        case ',': current_token_ = Token{TokenType::COMMA, ","}; break;
        case ':': current_token_ = Token{TokenType::COLON, ":"}; break;
        case '+':
            if (current_char_ == '=') {
                read_char();
                current_token_ = Token{TokenType::PLUS_EQUALS, "+="};
            } else {
                current_token_ = Token{TokenType::PLUS, "+"};
            }
            break;
        case '*':
            if (current_char_ == '=') {
                read_char();
                current_token_ = Token{TokenType::MULTIPLY_EQUALS, "*="};
            } else {
                current_token_ = Token{TokenType::MULTIPLY, "*"};
            }
            break;
        case '/':
            if (current_char_ == '/') {
                skip_line_comment();
                return next_token();
            }
            if (current_char_ == '=') {
                read_char();
                current_token_ = Token{TokenType::DIVIDE_EQUALS, "/="};
            } else {
                current_token_ = Token{TokenType::DIVIDE, "/"};
            }
            break;
        case '%':
            if (current_char_ == '=') {
                read_char();
                current_token_ = Token{TokenType::MODULO_EQUALS, "%="};
            } else {
                current_token_ = Token{TokenType::MODULO, "%"};
            }
            break;
        case '^':
            if (current_char_ == '=') {
                read_char();
                current_token_ = Token{TokenType::POWER_EQUALS, "^="};
            } else {
                current_token_ = Token{TokenType::POWER, "^"};
            }
            break;
        case '=':
            if (current_char_ == '=') {
                read_char();
                current_token_ = Token{TokenType::EQUAL_EQUAL, "=="};
            } else {
                current_token_ = Token{TokenType::EQUALS, "="};
            }
            break;
        case '!':
            if (current_char_ == '=') {
                read_char();
                current_token_ = Token{TokenType::NOT_EQUAL, "!="};
            } else {
                current_token_ = Token{TokenType::NOT, "!"};
            }
            break;
        case '<':
            if (current_char_ == '=') {
                read_char();
                current_token_ = Token{TokenType::LESS_EQUAL, "<="};
            } else {
                current_token_ = Token{TokenType::LESS, "<"};
            }
            break;
        case '>':
            if (current_char_ == '=') {
                read_char();
                current_token_ = Token{TokenType::GREATER_EQUAL, ">="};
            } else {
                current_token_ = Token{TokenType::GREATER, ">"};
            }
            break;
        case ';': current_token_ = Token{TokenType::SEMICOLON, ";"}; break;
        default: current_token_ = Token{TokenType::ERROR, std::string(1, c)}; break;
    }
    
    return current_token_;
} 