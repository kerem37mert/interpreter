#include "value.hpp"

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