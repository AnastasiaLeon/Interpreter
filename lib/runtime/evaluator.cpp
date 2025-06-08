#include "evaluator.h"
#include "operations.h"
#include "utils.h"
#include <stdexcept>
#include <algorithm>
#include <cstdlib>
#include <cmath>

std::vector<Value>* g_print_values = nullptr; // глобальный указатель для хранения значений для печати

Value evaluate(const ASTNode* node, Environment& env) { // вычисляет значение узла AST и возвращает результат типа Value
    // обработка функции
    if (auto fnNode = dynamic_cast<const FunctionNode*>(node)) {
        auto fval = std::make_shared<FunctionValue>();
        fval->parameters = fnNode->parameters;
        for (const auto& stmt : fnNode->body) {
            fval->body.push_back(stmt.get());
        }
        return fval;
    }
    // возврат null
    if (auto nullNode = dynamic_cast<const NullNode*>(node)) {
        return NullType{};
    }
    // возврат числа
    if (auto num = dynamic_cast<const NumberNode*>(node)) {
        return num->value;
    }
    // возврат строки
    if (auto str = dynamic_cast<const StringNode*>(node)) {
        return str->value;
    }
    // обработка вызова функции
    if (auto call = dynamic_cast<const CallNode*>(node)) {
        if (auto fn = dynamic_cast<const VariableNode*>(call->callee.get())) {
            const std::string& name = fn->name;
            // функция len
            if (name == "len" && call->arguments.size() == 1) {
                auto val = evaluate(call->arguments[0].get(), env);
                if (std::holds_alternative<std::string>(val)) {
                    return static_cast<double>(std::get<std::string>(val).size());
                } else if (std::holds_alternative<List>(val)) {
                    return static_cast<double>(std::get<List>(val)->elements.size());
                }
                throw std::runtime_error("Аргумент len() должен быть строкой или списком");
            }
            // функция range
            if (name == "range") {
                size_t argc = call->arguments.size();
                if (argc == 1) {
                    auto endVal = evaluate(call->arguments[0].get(), env);
                    if (!std::holds_alternative<double>(endVal)) throw std::runtime_error("Аргумент range() должен быть числом");
                    double end = std::get<double>(endVal);
                    auto result = std::make_shared<ListValue>();
                    for (double v = 0; v < end; v += 1) result->elements.push_back(v);
                    return result;
                } else if (argc == 2) {
                    auto startVal = evaluate(call->arguments[0].get(), env);
                    auto endVal = evaluate(call->arguments[1].get(), env);
                    if (!std::holds_alternative<double>(startVal) || !std::holds_alternative<double>(endVal)) throw std::runtime_error("Аргументы range() должны быть числами");
                    double start = std::get<double>(startVal);
                    double end = std::get<double>(endVal);
                    auto result = std::make_shared<ListValue>();
                    for (double v = start; v < end; v += 1) result->elements.push_back(v);
                    return result;
                } else if (argc == 3) {
                    auto startVal = evaluate(call->arguments[0].get(), env);
                    auto endVal = evaluate(call->arguments[1].get(), env);
                    auto stepVal = evaluate(call->arguments[2].get(), env);
                    if (!std::holds_alternative<double>(startVal) || !std::holds_alternative<double>(endVal) || !std::holds_alternative<double>(stepVal)) throw std::runtime_error("Аргументы range() должны быть числами");
                    double start = std::get<double>(startVal);
                    double end = std::get<double>(endVal);
                    double step = std::get<double>(stepVal);
                    auto result = std::make_shared<ListValue>();
                    if (step == 0) throw std::runtime_error("Шаг range() не может быть нулевым");
                    if (step > 0) {
                        for (double v = start; v < end; v += step) result->elements.push_back(v);
                    } else {
                        for (double v = start; v > end; v += step) result->elements.push_back(v);
                    }
                    return result;
                }
                throw std::runtime_error("range() ожидает 1, 2 или 3 аргумента");
            }
            if (name == "read" && call->arguments.empty()) {
                return std::string();
            }
            if (name == "stacktrace" && call->arguments.empty()) {
                auto result = std::make_shared<ListValue>();
                return result;
            }
            // математические функции
            if (name == "abs" && call->arguments.size() == 1) {
                auto v = evaluate(call->arguments[0].get(), env);
                if (!std::holds_alternative<double>(v)) throw std::runtime_error("Аргумент abs() должен быть числом");
                return std::fabs(std::get<double>(v));
            }
            if (name == "ceil" && call->arguments.size() == 1) {
                auto v = evaluate(call->arguments[0].get(), env);
                if (!std::holds_alternative<double>(v)) throw std::runtime_error("Аргумент ceil() должен быть числом");
                return std::ceil(std::get<double>(v));
            }
            if (name == "floor" && call->arguments.size() == 1) {
                auto v = evaluate(call->arguments[0].get(), env);
                if (!std::holds_alternative<double>(v)) throw std::runtime_error("Аргумент floor() должен быть числом");
                return std::floor(std::get<double>(v));
            }
            if (name == "round" && call->arguments.size() == 1) {
                auto v = evaluate(call->arguments[0].get(), env);
                if (!std::holds_alternative<double>(v)) throw std::runtime_error("Аргумент round() должен быть числом");
                return std::round(std::get<double>(v));
            }
            if (name == "sqrt" && call->arguments.size() == 1) {
                auto v = evaluate(call->arguments[0].get(), env);
                if (!std::holds_alternative<double>(v)) throw std::runtime_error("Аргумент sqrt() должен быть числом");
                return std::sqrt(std::get<double>(v));
            }
            if (name == "rnd" && call->arguments.size() == 1) {
                auto v = evaluate(call->arguments[0].get(), env);
                if (!std::holds_alternative<double>(v)) throw std::runtime_error("Аргумент rnd() должен быть числом");
                int n = static_cast<int>(std::get<double>(v));
                if (n <= 0) return 0.0;
                return static_cast<double>(std::rand() % n);
            }
            // работа со строками
            if (name == "parse_num" && call->arguments.size() == 1) {
                auto v = evaluate(call->arguments[0].get(), env);
                if (!std::holds_alternative<std::string>(v)) return NullType{};
                const std::string& s = std::get<std::string>(v);
                try {
                    double x = std::stod(s);
                    return x;
                } catch (...) {
                    return NullType{};
                }
            }
            if (name == "to_string" && call->arguments.size() == 1) {
                auto v = evaluate(call->arguments[0].get(), env);
                if (!std::holds_alternative<double>(v)) throw std::runtime_error("Аргумент to_string() должен быть числом");
                return format_number(std::get<double>(v));
            }
            if (name == "lower" && call->arguments.size() == 1) {
                auto v = evaluate(call->arguments[0].get(), env);
                if (!std::holds_alternative<std::string>(v)) throw std::runtime_error("Аргумент lower() должен быть строкой");
                std::string s = std::get<std::string>(v);
                for (char &c : s) c = std::tolower(c);
                return s;
            }
            if (name == "upper" && call->arguments.size() == 1) {
                auto v = evaluate(call->arguments[0].get(), env);
                if (!std::holds_alternative<std::string>(v)) throw std::runtime_error("Аргумент upper() должен быть строкой");
                std::string s = std::get<std::string>(v);
                for (char &c : s) c = std::toupper(c);
                return s;
            }
            if (name == "split" && call->arguments.size() == 2) {
                auto v = evaluate(call->arguments[0].get(), env);
                auto d = evaluate(call->arguments[1].get(), env);
                if (!std::holds_alternative<std::string>(v) || !std::holds_alternative<std::string>(d)) throw std::runtime_error("Аргументы split() должны быть строками");
                const std::string &str = std::get<std::string>(v);
                const std::string &delim = std::get<std::string>(d);
                auto result = std::make_shared<ListValue>();
                size_t start = 0, pos;
                while ((pos = str.find(delim, start)) != std::string::npos) {
                    result->elements.push_back(str.substr(start, pos - start));
                    start = pos + delim.length();
                }
                result->elements.push_back(str.substr(start));
                return result;
            }
            if (name == "join" && call->arguments.size() == 2) {
                auto v = evaluate(call->arguments[0].get(), env);
                auto d = evaluate(call->arguments[1].get(), env);
                if (!std::holds_alternative<List>(v) || !std::holds_alternative<std::string>(d)) throw std::runtime_error("Аргументы join() должны быть списком и строкой");
                const List &lst = std::get<List>(v);
                const std::string &delim = std::get<std::string>(d);
                std::string out;
                for (size_t i = 0; i < lst->elements.size(); ++i) {
                    const auto &elem = lst->elements[i];
                    if (!std::holds_alternative<std::string>(elem)) throw std::runtime_error("Элементы списка join() должны быть строками");
                    if (i > 0) out += delim;
                    out += std::get<std::string>(elem);
                }
                return out;
            }
            if (name == "replace" && call->arguments.size() == 3) {
                auto v = evaluate(call->arguments[0].get(), env);
                auto oldv = evaluate(call->arguments[1].get(), env);
                auto newv = evaluate(call->arguments[2].get(), env);
                if (!std::holds_alternative<std::string>(v) || !std::holds_alternative<std::string>(oldv) || !std::holds_alternative<std::string>(newv)) throw std::runtime_error("Аргументы replace() должны быть строками");
                std::string s = std::get<std::string>(v);
                const std::string &oldstr = std::get<std::string>(oldv);
                const std::string &newstr = std::get<std::string>(newv);
                size_t pos = 0;
                while ((pos = s.find(oldstr, pos)) != std::string::npos) {
                    s.replace(pos, oldstr.length(), newstr);
                    pos += newstr.length();
                }
                return s;
            }
            // работа со списками
            if (name == "push" && call->arguments.size() == 2) {
                auto v = evaluate(call->arguments[0].get(), env);
                if (!std::holds_alternative<List>(v)) throw std::runtime_error("Первый аргумент push() должен быть списком");
                List lst = std::get<List>(v);
                auto elem = evaluate(call->arguments[1].get(), env);
                lst->elements.push_back(elem);
                return NullType{};
            }
            if (name == "pop" && call->arguments.size() == 1) {
                auto v = evaluate(call->arguments[0].get(), env);
                if (!std::holds_alternative<List>(v)) throw std::runtime_error("Аргумент pop() должен быть списком");
                List lst = std::get<List>(v);
                if (lst->elements.empty()) return NullType{};
                Value last = lst->elements.back();
                lst->elements.pop_back();
                return last;
            }
            if (name == "insert" && call->arguments.size() == 3) {
                auto v = evaluate(call->arguments[0].get(), env);
                auto idxv = evaluate(call->arguments[1].get(), env);
                if (!std::holds_alternative<List>(v) || !std::holds_alternative<double>(idxv)) throw std::runtime_error("Аргументы insert() должны быть списком и индексом");
                List lst = std::get<List>(v);
                int idx = static_cast<int>(std::get<double>(idxv));
                if (idx < 0) idx = 0;
                if (idx > static_cast<int>(lst->elements.size())) idx = lst->elements.size();
                Value elem = evaluate(call->arguments[2].get(), env);
                lst->elements.insert(lst->elements.begin() + idx, elem);
                return NullType{};
            }
            if (name == "remove" && call->arguments.size() == 2) {
                auto v = evaluate(call->arguments[0].get(), env);
                auto idxv = evaluate(call->arguments[1].get(), env);
                if (!std::holds_alternative<List>(v) || !std::holds_alternative<double>(idxv)) throw std::runtime_error("Аргументы remove() должны быть списком и индексом");
                List lst = std::get<List>(v);
                int idx = static_cast<int>(std::get<double>(idxv));
                if (idx < 0 || idx >= static_cast<int>(lst->elements.size())) return NullType{};
                Value val = lst->elements[idx];
                lst->elements.erase(lst->elements.begin() + idx);
                return val;
            }
            if (name == "sort" && call->arguments.size() == 1) {
                auto v = evaluate(call->arguments[0].get(), env);
                if (!std::holds_alternative<List>(v)) throw std::runtime_error("Аргумент sort() должен быть списком");
                List lst = std::get<List>(v);
                bool allNum = true;
                for (auto &e : lst->elements) if (!std::holds_alternative<double>(e)) { allNum = false; break; }
                if (allNum) {
                    std::sort(lst->elements.begin(), lst->elements.end(), [](const Value&a,const Value&b){return std::get<double>(a)<std::get<double>(b);});
                } else {
                    bool allStr = true;
                    for (auto &e : lst->elements) if (!std::holds_alternative<std::string>(e)){ allStr=false; break; }
                    if (allStr) {
                        std::sort(lst->elements.begin(), lst->elements.end(), [](const Value&a,const Value&b){return std::get<std::string>(a)<std::get<std::string>(b);});
                    }
                }
                return NullType{};
            }
            // вызов пользовательской функции
            try {
                Value fn_val = env.get(name);
                if (std::holds_alternative<Function>(fn_val)) {
                    auto f = std::get<Function>(fn_val);
                    if (call->arguments.size() != f->parameters.size()) {
                        throw std::runtime_error("Несоответствие количества аргументов");
                    }
                    std::vector<Value> args;
                    for (const auto& arg : call->arguments) {
                        args.push_back(evaluate(arg.get(), env));
                    }
                    Environment local_env;
                    for (size_t i = 0; i < f->parameters.size() && i < args.size(); ++i) {
                        local_env.set(f->parameters[i], args[i]);
                    }
                    try {
                        for (const auto* stmt : f->body) {
                            execNode(stmt, local_env, *g_print_values);
                        }
                    } catch (const ReturnException& re) {
                        return re.value;
                    }
                    return NullType{};
                }
            } catch (const std::exception&) {
            }
        }
        // вызов функции через значение
        {
            Value calleeVal = evaluate(call->callee.get(), env);
            if (std::holds_alternative<Function>(calleeVal)) {
                auto f = std::get<Function>(calleeVal);
                if (call->arguments.size() != f->parameters.size()) {
                    throw std::runtime_error("Несоответствие количества аргументов");
                }
                std::vector<Value> args;
                for (const auto& arg : call->arguments) {
                    args.push_back(evaluate(arg.get(), env));
                }
                Environment local_env;
                for (size_t i = 0; i < f->parameters.size(); ++i) {
                    local_env.set(f->parameters[i], args[i]);
                }
                try {
                    for (const auto* stmt : f->body) {
                        execNode(stmt, local_env, *g_print_values);
                    }
                } catch (const ReturnException& re) {
                    return re.value;
                }
                return NullType{};
            }
        }
        throw std::runtime_error("Неизвестный вызов функции");
    }
    // создание списка
    if (auto listNode = dynamic_cast<const ListNode*>(node)) {
        auto result = std::make_shared<ListValue>();
        for (const auto& elem : listNode->elements) {
            auto value = evaluate(elem.get(), env);
            result->elements.push_back(value);
        }
        return result;
    }
    // получение переменной
    if (auto var = dynamic_cast<const VariableNode*>(node)) {
        return env.get(var->name);
    }
    // бинарная операция
    if (auto binary = dynamic_cast<const BinaryOpNode*>(node)) {
        auto left = evaluate(binary->left.get(), env);
        auto right = evaluate(binary->right.get(), env);
        return apply_binary_op(left, right, binary->op);
    }
    // унарная операция
    if (auto unary = dynamic_cast<const UnaryOpNode*>(node)) {
        auto operand = evaluate(unary->operand.get(), env);
        return apply_unary_op(unary->op, operand);
    }
    // присваивание
    if (auto assign = dynamic_cast<const AssignNode*>(node)) {
        auto value = evaluate(assign->value.get(), env);
        if (assign->op == TokenType::EQUALS) {
            env.set(assign->var_name, value);
        } else {
            auto current = env.get(assign->var_name);
            TokenType op = get_binary_op_from_compound_assign(assign->op);
            value = apply_binary_op(current, value, op);
            env.set(assign->var_name, value);
        }
        return value;
    }
    // индексация
    if (auto index = dynamic_cast<const IndexNode*>(node)) {
        auto container_val = evaluate(index->str.get(), env);
        auto idx_val = evaluate(index->index.get(), env);
        if (!std::holds_alternative<double>(idx_val)) {
            throw std::runtime_error("Индекс должен быть числом");
        }
        int idx = static_cast<int>(std::get<double>(idx_val));
        if (std::holds_alternative<std::string>(container_val)) {
            const std::string& str = std::get<std::string>(container_val);
            int len = static_cast<int>(str.length());
            if (idx < 0) idx = len + idx;
            if (idx < 0 || idx >= len) {
                return NullType{};
            }
            return std::string(1, str[idx]);
        } else if (std::holds_alternative<List>(container_val)) {
            const List& list = std::get<List>(container_val);
            int len = static_cast<int>(list->elements.size());
            if (idx < 0) idx = len + idx;
            if (idx < 0 || idx >= len) {
                return NullType{};
            }
            return list->elements[idx];
        } else {
            throw std::runtime_error("Операция индексации требует строку или список");
        }
    }
    // срез
    if (auto slice = dynamic_cast<const SliceNode*>(node)) {
        auto container_val = evaluate(slice->str.get(), env);
        if (std::holds_alternative<std::string>(container_val)) {
            const std::string& str = std::get<std::string>(container_val);
            int len = static_cast<int>(str.length());
            int start = 0;
            int end = len;
            if (slice->start) {
                auto start_val = evaluate(slice->start.get(), env);
                if (!std::holds_alternative<double>(start_val)) {
                    throw std::runtime_error("Индексы среза должны быть числами");
                }
                start = static_cast<int>(std::get<double>(start_val));
                if (start < 0) {
                    start = len + start;
                }
            }
            if (slice->end) {
                auto end_val = evaluate(slice->end.get(), env);
                if (!std::holds_alternative<double>(end_val)) {
                    throw std::runtime_error("Индексы среза должны быть числами");
                }
                end = static_cast<int>(std::get<double>(end_val));
                if (end < 0) {
                    end = len + end;
                }
            }
            start = std::max(0, std::min(start, len));
            end = std::max(0, std::min(end, len));
            if (start > end) {
                return std::string();
            }
            return str.substr(start, end - start);
        } else if (std::holds_alternative<List>(container_val)) {
            const List& list = std::get<List>(container_val);
            int len = static_cast<int>(list->elements.size());
            int start = 0;
            int end = len;
            if (slice->start) {
                auto start_val = evaluate(slice->start.get(), env);
                if (!std::holds_alternative<double>(start_val)) {
                    throw std::runtime_error("Индексы среза должны быть числами");
                }
                start = static_cast<int>(std::get<double>(start_val));
                if (start < 0) {
                    start = len + start;
                }
            }
            if (slice->end) {
                auto end_val = evaluate(slice->end.get(), env);
                if (!std::holds_alternative<double>(end_val)) {
                    throw std::runtime_error("Индексы среза должны быть числами");
                }
                end = static_cast<int>(std::get<double>(end_val));
                if (end < 0) {
                    end = len + end;
                }
            }
            start = std::max(0, std::min(start, len));
            end = std::max(0, std::min(end, len));
            if (start > end) {
                return std::make_shared<ListValue>();
            }
            auto result = std::make_shared<ListValue>();
            result->elements.insert(result->elements.end(), list->elements.begin() + start, list->elements.begin() + end);
            return result;
        } else {
            throw std::runtime_error("Операция среза требует строку или список");
        }
    }
    // печать
    if (auto print = dynamic_cast<const PrintNode*>(node)) {
        return evaluate(print->expr.get(), env);
    }
    throw std::runtime_error("Неизвестный тип узла");
}

// выполнение узла
void execNode(const ASTNode* node, Environment& env, std::vector<Value>& print_values) { // выполняет узел AST, изменяя среду выполнения или добавляя значения в print_values.
    if (auto retNode = dynamic_cast<const ReturnNode*>(node)) {
        Value val = evaluate(retNode->expr.get(), env);
        throw ReturnException{val};
    }
    if (auto call = dynamic_cast<const CallNode*>(node)) {
        if (auto fn = dynamic_cast<const VariableNode*>(call->callee.get())) {
            const std::string& name = fn->name;
            // функции print и println
            if (name == "print" && call->arguments.size() == 1) {
                auto value = evaluate(call->arguments[0].get(), env);
                print_values.push_back(value);
                return;
            }
            if (name == "println" && call->arguments.size() == 1) {
                auto value = evaluate(call->arguments[0].get(), env);
                print_values.push_back(value);
                print_values.push_back(std::string("\n"));
                return;
            }
        }
    }
    // прерывание цикла
    if (dynamic_cast<const BreakNode*>(node)) {
        throw BreakException();
    }
    // продолжение цикла
    if (dynamic_cast<const ContinueNode*>(node)) {
        throw ContinueException();
    }
    // обработка печати
    if (auto printNode = dynamic_cast<const PrintNode*>(node)) {
        if (auto lit = dynamic_cast<const StringNode*>(printNode->expr.get())) {
            auto rawVal = evaluate(printNode->expr.get(), env);
            if (!std::holds_alternative<std::string>(rawVal)) {
                throw std::runtime_error("Ожидался строковый литерал");
            }
            const std::string raw = std::get<std::string>(rawVal);
            std::string out = raw;
            if (raw.find(' ') != std::string::npos) {
                out = std::string("\"") + raw + "\"";
            }
            print_values.push_back(out);
        } else {
            auto value = evaluate(printNode->expr.get(), env);
            print_values.push_back(value);
        }
    } else if (auto assignNode = dynamic_cast<const AssignNode*>(node)) {
        evaluate(assignNode, env);
    } else if (auto ifNode = dynamic_cast<const IfNode*>(node)) {
        // условный оператор
        bool executed = false;
        for (const auto& branch : ifNode->branches) {
            Value cond = evaluate(branch.first.get(), env);
            if (isTruthy(cond)) {
                for (const auto& stmt : branch.second) execNode(stmt.get(), env, print_values);
                executed = true;
                break;
            }
        }
        if (!executed) {
            for (const auto& stmt : ifNode->else_branch) execNode(stmt.get(), env, print_values);
        }
    } else if (auto forNode = dynamic_cast<const ForNode*>(node)) {
        // цикл for
        Value iterable = evaluate(forNode->iterable.get(), env);
        if (!std::holds_alternative<List>(iterable)) throw std::runtime_error("Итерируемый объект цикла for должен быть списком");
        const List& listVal = std::get<List>(iterable);
        for (const auto& elem : listVal->elements) {
            env.set(forNode->var_name, elem);
            try {
                for (const auto& stmt : forNode->body) execNode(stmt.get(), env, print_values);
            } catch (const ContinueException&) {
                continue;
            } catch (const BreakException&) {
                break;
            }
        }
    } else if (auto whileNode = dynamic_cast<const WhileNode*>(node)) {
        // цикл while
        while (true) {
            Value cond = evaluate(whileNode->condition.get(), env);
            if (!isTruthy(cond)) break;
            try {
                for (const auto& stmt : whileNode->body) execNode(stmt.get(), env, print_values);
            } catch (const ContinueException&) {
                continue;
            } catch (const BreakException&) {
                break;
            }
        }
    } else {
        evaluate(node, env);
    }
}