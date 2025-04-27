#ifndef VALUE_HPP
#define VALUE_HPP

#include <string>
#include <vector>
#include <variant>

class Value {
public:
    using Type = std::variant<std::monostate, bool, double, std::string, std::vector<Value>>;

    Value();
    explicit Value(bool value);
    explicit Value(double value);
    explicit Value(const std::string& value);
    explicit Value(const std::vector<Value>& values);

private:
    Type type;
};

#endif //VALUE_HPP
