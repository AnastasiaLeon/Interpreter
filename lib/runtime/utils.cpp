#include "utils.h"
#include <sstream>
#include <iomanip>

std::string format_number(double num) { // преобразование числа в праивьную строку (без 0 в конце)
    if (num == static_cast<int>(num)) {
        return std::to_string(static_cast<int>(num));
    }
    
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(6) << num;
    std::string str = ss.str();
    
    while (str.back() == '0') {
        str.pop_back();
    }
    
    if (str.back() == '.') {
        str.pop_back();
    }
    
    return str;
}

bool isTruthy(const Value& v) { // является ли значение истинным в логическом контексте
    if (std::holds_alternative<NullType>(v)) return false;
    if (std::holds_alternative<double>(v)) return std::get<double>(v) != 0.0;
    if (std::holds_alternative<std::string>(v)) return !std::get<std::string>(v).empty();
    if (std::holds_alternative<List>(v)) return !std::get<List>(v)->elements.empty();
    return false;
}