#include "value.hpp"

Value::Value()
    : type(std::monostate{}) {}

Value::Value(bool value)
    : type(value) {}

Value::Value(double value)
    : type(value) {}

Value::Value(const std::string& value)
    : type(value) {}

Value::Value(const std::vector<Value>& values)
    : type(values) {}
