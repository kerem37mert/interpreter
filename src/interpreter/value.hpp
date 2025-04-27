#ifndef VALUE_HPP
#define VALUE_HPP

#include <string>
#include <vector>
#include <variant>

class Value {
public:
    using ValueType = std::variant<std::monostate, bool, double, std::string, std::vector<Value>>;

    Value();
    explicit Value(bool value);
    explicit Value(double value);
    explicit Value(const std::string& value);
    explicit Value(const std::vector<Value>& values);

    bool isNil() const;
    bool isBool() const;
    bool isNumber() const;
    bool isString() const;
    bool isArray() const;

    bool asBool() const;
    double asNumber() const;
    const std::string& asString() const;
    const std::vector<Value>& asArray() const;

    std::string toString() const;

private:
    ValueType value;
};

#endif //VALUE_HPP
