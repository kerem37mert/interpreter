#include "value.hpp"
#include <sstream>

Value::Value()
    : value(std::monostate{}) {}

Value::Value(bool value)
    : value(value) {}

Value::Value(double value)
    : value(value) {}

Value::Value(const std::string& value)
    : value(value) {}

Value::Value(const std::vector<Value>& values)
    : value(values) {}

bool Value::isNil() const {
    return std::holds_alternative<std::monostate>(this->value);
}

bool Value::isBool() const {
    return std::holds_alternative<bool>(this->value);
}

bool Value::isNumber() const {
    return std::holds_alternative<double>(this->value);
}

bool Value::isString() const {
    return std::holds_alternative<std::string>(this->value);
}

bool Value::isArray() const {
    return std::holds_alternative<std::vector<Value>>(this->value);
}

bool Value::asBool() const {
    return std::get<bool>(this->value);
}

double Value::asNumber() const {
    return std::get<double>(this->value);
}

const std::string& Value::asString() const {
    return std::get<std::string>(this->value);
}

const std::vector<Value>& Value::asArray() const {
    return std::get<std::vector<Value>>(this->value);
}

std::string Value::toString() const {
    if(this->isNil())
        return "nil";

    if(this->isBool())
        return this->asBool() ? "doğru" : "yanlış";

    if(this->isNumber()) {
        std::ostringstream oss;
        oss << this->asNumber();
        return oss.str();
    }

    if(this->isString()) {
        std::string str = this->asString();
        if (!str.empty() && str.front() == '"' && str.back() == '"') {
            return str.substr(1, str.length() - 2);
        }
        return str;
    }

    if(this->isArray()) {
        std::string result = "[";
        const std::vector<Value>& array = this->asArray();

        for (size_t i = 0; i < array.size(); ++i) {
            if (i > 0) {
                result += ", ";
            }
            result += array[i].toString();
        }

        result += "]";
        return result;
    }

    return "bilinmeyen";
}