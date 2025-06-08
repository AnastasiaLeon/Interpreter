#pragma once
#include "types.h"
#include <unordered_map>
#include <stdexcept>

class Environment { // среда выполнения которая хранит переменные, их значения, обеспечивает к переменным доступ по имени, управляет областью видимости
public:
    void set(const std::string& name, const Value& value) { // переменная
        variables_[name] = value;
    }

    Value get(const std::string& name) const { // получение значения переменной
        auto it = variables_.find(name);
        if (it == variables_.end()) {
            throw std::runtime_error("Неопределенная переменная: " + name);
        }
        return it->second;
    }

private:
    std::unordered_map<std::string, Value> variables_; // хт с именем переменной - значением
};