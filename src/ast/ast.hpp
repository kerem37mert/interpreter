#ifndef AST_H
#define AST_H

#include <memory>
#include "../token/token.hpp"

class AstNode {
public:
    virtual ~AstNode() = default;
};

class AstNodeIf : public AstNode {
public:
};

class AstNodeLoop : public AstNode {
public:
};

class AstNodeFunction : public AstNode {
public:
};


#endif //AST_H