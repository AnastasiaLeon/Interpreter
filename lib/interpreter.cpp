#include "interpreter.h"
#include "parser/parser.h"
#include "runtime/evaluator.h"
#include "runtime/environment.h"
#include "runtime/utils.h"
#include "runtime/types.h"
#include <stdexcept>

bool interpret(std::istream& input, std::ostream& output) {
    try {
        Lexer lexer(input);
        Parser parser(lexer);
        auto ast = parser.parse();
        Environment env;

        std::vector<Value> print_values;
        g_print_values = &print_values;
        for (const auto& node : ast) {
            execNode(node.get(), env, print_values);
        }
        g_print_values = nullptr;

        for (size_t i = 0; i < print_values.size(); ++i) {
            const auto& value = print_values[i];
            if (std::holds_alternative<NullType>(value)) {
                output << "nil";
                continue;
            }
            if (std::holds_alternative<double>(value)) {
                output << format_number(std::get<double>(value));
            } else if (std::holds_alternative<std::string>(value)) {
                output << std::get<std::string>(value);
            } else if (std::holds_alternative<List>(value)) {
                const List& list = std::get<List>(value);
                output << "[";
                for (size_t j = 0; j < list->elements.size(); ++j) {
                    if (j > 0) {
                        output << ", ";
                    }
                    const Value& elem = list->elements[j];
                    if (std::holds_alternative<NullType>(elem)) {
                        output << "nil";
                    } else if (std::holds_alternative<double>(elem)) {
                        output << format_number(std::get<double>(elem));
                    } else if (std::holds_alternative<std::string>(elem)) {
                        output << "\"" << std::get<std::string>(elem) << "\"";
                    } else if (std::holds_alternative<List>(elem)) {
                        const List& nested = std::get<List>(elem);
                        output << "[";
                        for (size_t k = 0; k < nested->elements.size(); ++k) {
                            if (k > 0) output << ", ";
                            const Value& nelem = nested->elements[k];
                            if (std::holds_alternative<NullType>(nelem)) {
                                output << "nil";
                            } else if (std::holds_alternative<double>(nelem)) {
                                output << format_number(std::get<double>(nelem));
                            } else if (std::holds_alternative<std::string>(nelem)) {
                                output << "\"" << std::get<std::string>(nelem) << "\"";
                            }
                        }
                        output << "]";
                    }
                }
                output << "]";
            }
        }
        return true;
    } catch (const std::exception& e) {
        output << "Ошибка: " << e.what();
        return false;
    }
}