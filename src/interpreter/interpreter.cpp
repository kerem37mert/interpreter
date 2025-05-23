#include "interpreter.hpp"

#include <cmath>
#include <iostream>

void Interpreter::Environment::define(const std::string& name, const Value& value, bool isConst) {
    if(this->variables.find(name) != this->variables.end())
        throw std::runtime_error("Bu isimde bir değişken zaten tanımlı: " + name);

    this->variables[name] = value;
    this->constants[name] = isConst;
}

void Interpreter::Environment::defineFunction(const std::string& name, FunctionDeclStmt* function) {
    this->functions[name] = function;
}

Value Interpreter::Environment::get(const std::string& name) {
    auto it = this->variables.find(name);

    if(it != this->variables.end())
        return it->second;

    if(this->enclosing != nullptr)
        return this->enclosing->get(name);

    throw std::runtime_error("Tanımsız değişken: " + name);
}

FunctionDeclStmt* Interpreter::Environment::getFunction(const std::string& name) {
    auto it = this->functions.find(name);

    if(it != this->functions.end())
        return it->second;

    throw std::runtime_error("Tanımsız fonksiyon: " + name);
}

void Interpreter::Environment::assign(const std::string& name, const Value& value) {
    auto it = this->variables.find(name);

    if (it != variables.end()) {
        auto constIt = this->constants.find(name);
        if (constIt != this->constants.end() && constIt->second)
            throw std::runtime_error("Sabit bir değişkene değer atanamaz: " + name);

        // Tip kontrolü
        Value currentValue = it->second;
        if (currentValue.isNumber() && !value.isNumber())
            throw std::runtime_error("Sayı tipindeki değişkene sayı olmayan bir değer atanamaz: " + name);
        if (currentValue.isString() && !value.isString())
            throw std::runtime_error("Metin tipindeki değişkene metin olmayan bir değer atanamaz: " + name);
        if (currentValue.isBool() && !value.isBool())
            throw std::runtime_error("Boolean tipindeki değişkene boolean olmayan bir değer atanamaz: " + name);
        if (currentValue.isArray() && !value.isArray())
            throw std::runtime_error("Dizi tipindeki değişkene dizi olmayan bir değer atanamaz: " + name);

        this->variables[name] = value;
        return;
    }

    if (enclosing != nullptr) {
        enclosing->assign(name, value);
        return;
    }

    throw std::runtime_error("Tanımsız değişken: " + name);
}

Interpreter::Interpreter() {
    this->currentEnvironment = new Environment();
    environmentStack.push(currentEnvironment);
}

void Interpreter::interpret(AstNode* node) {
    try {
        node->accept(*this);
    } catch (const std::exception& e) {
        std::cerr << "[HATA] " << e.what() << std::endl;
    }
}

void Interpreter::pushEnvironment() {
    this->currentEnvironment = new Environment(currentEnvironment);
    this->environmentStack.push(this->currentEnvironment);
}


void Interpreter::popEnvironment() {
    Environment* old = this->environmentStack.top();
    this->environmentStack.pop();
    this->currentEnvironment = this->environmentStack.top();
    delete old;
}

void Interpreter::enterBlock() {
    this->pushEnvironment();
}

void Interpreter::exitBlock() {
    this->popEnvironment();
}

Value Interpreter::evaluate(Expression* expr) {
    expr->accept(*this);
    return result;
}

void Interpreter::execute(Stmt* stmt) {
    stmt->accept(*this);
}

bool Interpreter::isTruthy(const Value& value) {
    if (value.isNil())
        return false;

    if (value.isBool())
        return value.asBool();

    return true;
}

bool Interpreter::isEqual(const Value& a, const Value& b) {
    if (a.isNil() && b.isNil())
        return true;

    if (a.isNil())
        return false;

    if (a.isBool() && b.isBool())
        return a.asBool() == b.asBool();

    if (a.isNumber() && b.isNumber())
        return a.asNumber() == b.asNumber();

    if (a.isString() && b.isString())
        return a.asString() == b.asString();

    return false;
}

void Interpreter::checkNumberOperand(const Token& op, const Value& operand) {
    if (operand.isNumber())
        return;

    throw std::runtime_error("Operand sayı olmalıdır.");
}

void Interpreter::checkNumberOperands(const Token& op, const Value& left, const Value& right) {
    if (left.isNumber() && right.isNumber())
        return;

    throw std::runtime_error("Operandlar sayı olmalıdır.");
}

void Interpreter::checkTypeCompatibility(const std::string& type, const Value& value) {
    if (type == "sayı" && !value.isNumber())
        throw std::runtime_error("Sayı tipindeki değişkene sayı olmayan bir değer atanamaz.");

    if (type == "metin" && !value.isString())
        throw std::runtime_error("Metin tipindeki değişkene metin olmayan bir değer atanamaz.");

    if (type == "boolean" && !value.isBool())
        throw std::runtime_error("Boolean tipindeki değişkene boolean olmayan bir değer atanamaz.");

    if (type == "dizi" && !value.isArray())
        throw std::runtime_error("Dizi tipindeki değişkene dizi olmayan bir değer atanamaz.");
}

void Interpreter::visitBinaryExpression(BinaryExpression* expr) {
    Value left = this->evaluate(expr->left.get());
    Value right = this->evaluate(expr->right.get());

    std::string op(expr->op.start, expr->op.length);

    if (op == "+") {
        if (left.isNumber() && right.isNumber()) {
            this->result = Value(left.asNumber() + right.asNumber());
        } else if (left.isString() && right.isString()) {
            this->result = Value(left.asString() + right.asString());
        } else if (left.isString() && right.isNumber()) {
            this->result = Value(left.asString() + std::to_string(right.asNumber()));
        } else if (left.isNumber() && right.isString()) {
            this->result = Value(std::to_string(left.asNumber()) + right.asString());
        } else if (left.isString() && right.isBool()) {
            this->result = Value(left.asString() + (right.asBool() ? "doğru" : "yanlış"));
        } else if (left.isBool() && right.isString()) {
            this->result = Value((left.asBool() ? "doğru" : "yanlış") + right.asString());
        } else {
            throw std::runtime_error("Operandlar sayı, metin veya doğruluk değeri olmalıdır.");
        }
    } else if (op == "-") {
        this->checkNumberOperands(expr->op, left, right);
        this->result = Value(left.asNumber() - right.asNumber());
    } else if (op == "*") {
        this->checkNumberOperands(expr->op, left, right);
        this->result = Value(left.asNumber() * right.asNumber());
    } else if (op == "/") {
        this->checkNumberOperands(expr->op, left, right);
        if (right.asNumber() == 0)
            throw std::runtime_error("Sıfıra bölme hatası.");

        this->result = Value(left.asNumber() / right.asNumber());
    } else if (op == "%") {
        this->checkNumberOperands(expr->op, left, right);
        if (right.asNumber() == 0)
            throw std::runtime_error("Sıfıra bölme hatası.");

        this->result = Value(fmod(left.asNumber(), right.asNumber()));
    }
}

void Interpreter::visitLogicalExpression(LogicalExpression* expr) {
    Value left = evaluate(expr->left.get());

    std::string op(expr->op.start, expr->op.length);

    if(op == "&&") {
        if (!this->isTruthy(left)) {
            result = left;
            return;
        }

        this->result = this->evaluate(expr->right.get());
    } else if(op == "||") {
        if (this->isTruthy(left)) {
            this->result = left;
            return;
        }

        this->result = this->evaluate(expr->right.get());
    }
}

void Interpreter::visitUnaryExpression(UnaryExpression* expr) {
    Value right = this->evaluate(expr->operand.get());

    std::string op(expr->op.start, expr->op.length);

    if(op == "-") {
        this->checkNumberOperand(expr->op, right);
        this->result = Value(-right.asNumber());
    } else if(op == "!") {
        this->result = Value(!isTruthy(right));
    }
}

void Interpreter::visitComparisonExpression(ComparisonExpression* expr) {
    Value left = this->evaluate(expr->left.get());
    Value right = this->evaluate(expr->right.get());

    std::string op(expr->op.start, expr->op.length);

    if (op == ">") {
        this->checkNumberOperands(expr->op, left, right);
        this->result = Value(left.asNumber() > right.asNumber());
    } else if(op == ">=") {
        this->checkNumberOperands(expr->op, left, right);
        this->result = Value(left.asNumber() >= right.asNumber());
    } else if(op == "<") {
        this->checkNumberOperands(expr->op, left, right);
        this->result = Value(left.asNumber() < right.asNumber());
    } else if(op == "<=") {
        this->checkNumberOperands(expr->op, left, right);
        this->result = Value(left.asNumber() <= right.asNumber());
    } else if(op == "==")
        this->result = Value(isEqual(left, right));
    else if(op == "!=")
        this->result = Value(!isEqual(left, right));
}

void Interpreter::visitLiteralExpression(LiteralExpression* expr) {
    switch(expr->token.type) {
    case TokenType::NUMBER_LITERAL:
        this->result = Value(std::stod(expr->value));
        break;
    case TokenType::STRING_LITERAL:
        this->result = Value(expr->value);
        break;
    case TokenType::TRUE:
        this->result = Value(true);
        break;
    case TokenType::FALSE:
        this->result = Value(false);
        break;
    default:
        this->result = Value();
    }
}

void Interpreter::visitVariableExpression(VariableExpression* expr) {
    std::string name(expr->name.start, expr->name.length);
    this->result = this->currentEnvironment->get(name);
}

void Interpreter::visitAssignExpression(AssignExpression* expr) {
    Value value = this->evaluate(expr->value.get());
    std::string name(expr->name.start, expr->name.length);
    this->currentEnvironment->assign(name, value);
    this->result = value;
}

void Interpreter::visitCallExpression(CallExpression* expr) {
    // Fonksiyon adını al
    VariableExpression* varExpr = dynamic_cast<VariableExpression*>(expr->callee.get());
    if (!varExpr) {
        throw std::runtime_error("Fonksiyon çağrısı geçersiz.");
    }
    std::string calleeName(varExpr->name.start, varExpr->name.length);

    // Fonksiyonu bul
    FunctionDeclStmt* function = nullptr;
    Environment* env = this->currentEnvironment;
    while (env != nullptr) {
        try {
            function = env->getFunction(calleeName);
            break;
        } catch (const std::runtime_error&) {
            env = env->enclosing;
        }
    }

    if (!function) {
        throw std::runtime_error("Tanımsız fonksiyon: " + calleeName);
    }

    // Argümanları değerlendir
    std::vector<Value> arguments;
    for (auto& arg : expr->arguments) {
        arguments.push_back(evaluate(arg.get()));
    }

    // Argüman sayısı kontrolü
    if (arguments.size() != function->paramNames.size()) {
        throw std::runtime_error("Fonksiyon çağrısında argüman sayısı uyuşmuyor. Beklenen: " +
            std::to_string(function->paramNames.size()) + ", Verilen: " +
            std::to_string(arguments.size()));
    }

    // Yeni bir çevre oluştur
    this->pushEnvironment();

    // Parametreleri yeni çevreye ekle
    for (size_t i = 0; i < function->paramNames.size(); i++) {
        std::string paramName(function->paramNames[i].start, function->paramNames[i].length);
        std::string paramType(function->paramTypes[i].start, function->paramTypes[i].length);

        // Tip kontrolü
        this->checkTypeCompatibility(paramType, arguments[i]);

        this->currentEnvironment->define(paramName, arguments[i]);
    }

    // Fonksiyon gövdesini çalıştır
    try {
        this->execute(function->body.get());
    } catch (const std::runtime_error& e) {
        if (std::string(e.what()) == "RETURN") {
            // Return değeri zaten result'ta, bir şey yapmaya gerek yok
        } else {
            this->popEnvironment();
            throw e;
        }
    }

    // Çevreyi temizle
    this->popEnvironment();
}

void Interpreter::visitArrayExpression(ArrayExpression* expr) {
    std::vector<Value> elements;
    for (auto& element : expr->elements) {
        elements.push_back(evaluate(element.get()));
    }
    this->result = Value(elements);
}

void Interpreter::visitArrayAccessExpression(ArrayAccessExpression* expr) {
    Value array = this->evaluate(expr->array.get());
    Value index = this->evaluate(expr->index.get());

    if (!array.isArray())
        throw std::runtime_error("Dizi olmayan bir değere erişim yapılamaz.");

    if (!index.isNumber())
        throw std::runtime_error("Dizi indeksi sayı olmalıdır.");

    int idx = static_cast<int>(index.asNumber());
    const std::vector<Value>& elements = array.asArray();

    if (idx < 0 || idx >= elements.size())
        throw std::runtime_error("Dizi indeksi sınırlar dışında.");

    this->result = elements[idx];
}

/*void Interpreter::visitArrayAssignExpression(ArrayAssignExpression* expr) {
    Value array = this->evaluate(expr->array.get());
    Value index = this->evaluate(expr->index.get());
    Value value = this->evaluate(expr->value.get());

    if (!array.isArray())
        throw std::runtime_error("Dizi olmayan bir değere atama yapılamaz.");

    if (!index.isNumber())
        throw std::runtime_error("Dizi indeksi sayı olmalıdır.");

    // TODO: Dizi atama implementasyonu
    this->result = value;
}*/

void Interpreter::visitExpressionStmt(ExpressionStmt* stmt) {
    this->evaluate(stmt->expression.get());
}

void Interpreter::visitPrintStmt(PrintStmt* stmt) {
    Value value = this->evaluate(stmt->expression.get());
    std::cout << value.toString() << std::endl;
}

void Interpreter::visitVarDeclStmt(VarDeclStmt* stmt) {
    Value value;

    if(stmt->initializer != nullptr) {
        value = this->evaluate(stmt->initializer.get());
        std::string type(stmt->type.start, stmt->type.length);
        this->checkTypeCompatibility(type, value);
    }

    std::string name(stmt->name.start, stmt->name.length);
    this->currentEnvironment->define(name, value, stmt->isConst);
}

void Interpreter::visitBlockStmt(BlockStmt* stmt) {
    this->enterBlock();

    for(auto& stmt : stmt->statements)
        this->execute(stmt.get());

    this->exitBlock();
}

void Interpreter::visitIfStmt(IfStmt* stmt) {
    if (this->isTruthy(this->evaluate(stmt->condition.get())))
        this->execute(stmt->thenBranch.get());
    else if (stmt->elseBranch != nullptr)
        this->execute(stmt->elseBranch.get());
}

void Interpreter::visitLoopStmt(LoopStmt* stmt) {
    while(this->isTruthy(this->evaluate(stmt->condition.get())))
        this->execute(stmt->body.get());
}

void Interpreter::visitBreakStmt(BreakStmt* stmt) {

}

void Interpreter::visitContinueStmt(ContinueStmt* stmt) {

}

void Interpreter::visitFunctionDeclStmt(FunctionDeclStmt* stmt) {
    std::string name(stmt->name.start, stmt->name.length);
    // Fonksiyonu global scope'ta tanımla
    this->environmentStack.top()->defineFunction(name, stmt);
}

void Interpreter::visitReturnStmt(ReturnStmt* stmt) {
    Value value;
    if (stmt->value != nullptr) {
        value = this->evaluate(stmt->value.get());
    }
    this->result = value;
    throw std::runtime_error("RETURN");
}

void Interpreter::visitProgram(Program* program) {
    for(auto& stmt : program->statements)
        this->execute(stmt.get());
}
