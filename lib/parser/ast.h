#pragma once
#include <memory>
#include <string>
#include <vector>
#include "lexer/lexer.h"

struct ASTNode {
    virtual ~ASTNode() = default;
};

struct NumberNode : ASTNode {
    double value;
    explicit NumberNode(double v) : value(v) {}
};

struct StringNode : ASTNode {
    std::string value;
    explicit StringNode(std::string v) : value(std::move(v)) {}
};

struct NullNode : ASTNode {
    NullNode() = default;
};

// узел AST для переменной
struct VariableNode : ASTNode {
    std::string name;
    explicit VariableNode(std::string n) : name(std::move(n)) {}
};

struct BinaryOpNode : ASTNode {
    TokenType op;
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;
    BinaryOpNode(TokenType o, std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r)
        : op(o), left(std::move(l)), right(std::move(r)) {}
};

struct UnaryOpNode : ASTNode {
    TokenType op;
    std::unique_ptr<ASTNode> operand;
    UnaryOpNode(TokenType o, std::unique_ptr<ASTNode> expr)
        : op(o), operand(std::move(expr)) {}
};

struct AssignNode : ASTNode {
    std::string var_name;
    TokenType op;  // оператор присваивания или составного присваивания
    std::unique_ptr<ASTNode> value;
    AssignNode(std::string name, TokenType o, std::unique_ptr<ASTNode> v)
        : var_name(std::move(name)), op(o), value(std::move(v)) {}
};

struct PrintNode : ASTNode {
    std::unique_ptr<ASTNode> expr;
    explicit PrintNode(std::unique_ptr<ASTNode> e) : expr(std::move(e)) {}
};

struct IndexNode : ASTNode {
    std::unique_ptr<ASTNode> str;
    std::unique_ptr<ASTNode> index;
    IndexNode(std::unique_ptr<ASTNode> s, std::unique_ptr<ASTNode> i)
        : str(std::move(s)), index(std::move(i)) {}
};

struct SliceNode : ASTNode {
    std::unique_ptr<ASTNode> str;
    std::unique_ptr<ASTNode> start;  // может быть nullptr для значения по умолчанию
    std::unique_ptr<ASTNode> end;    // может быть nullptr для значения по умолчанию
    SliceNode(std::unique_ptr<ASTNode> s, std::unique_ptr<ASTNode> st, std::unique_ptr<ASTNode> e)
        : str(std::move(s)), start(std::move(st)), end(std::move(e)) {}
};

struct ListNode : ASTNode {
    std::vector<std::unique_ptr<ASTNode>> elements;
    explicit ListNode(std::vector<std::unique_ptr<ASTNode>> elements) : elements(std::move(elements)) {}
};

// узел AST для вызовов функций
struct CallNode : ASTNode {
    std::unique_ptr<ASTNode> callee;
    std::vector<std::unique_ptr<ASTNode>> arguments;
    CallNode(std::unique_ptr<ASTNode> c, std::vector<std::unique_ptr<ASTNode>> args)
        : callee(std::move(c)), arguments(std::move(args)) {}
};

// узел AST для условных операторов (if/else if/else)
struct IfNode : ASTNode {
    // ветви: пара условия и операторов
    std::vector<std::pair<std::unique_ptr<ASTNode>, std::vector<std::unique_ptr<ASTNode>>>> branches;
    // операторы для необязательной ветви else
    std::vector<std::unique_ptr<ASTNode>> else_branch;
    IfNode(
        std::vector<std::pair<std::unique_ptr<ASTNode>, std::vector<std::unique_ptr<ASTNode>>>> b,
        std::vector<std::unique_ptr<ASTNode>> eb)
        : branches(std::move(b)), else_branch(std::move(eb)) {}
};

// узел AST для циклов for
struct ForNode : ASTNode {
    std::string var_name;
    std::unique_ptr<ASTNode> iterable;
    std::vector<std::unique_ptr<ASTNode>> body;
    ForNode(std::string name, std::unique_ptr<ASTNode> it, std::vector<std::unique_ptr<ASTNode>> b)
        : var_name(std::move(name)), iterable(std::move(it)), body(std::move(b)) {}
};

// узел AST для циклов while
struct WhileNode : ASTNode {
    std::unique_ptr<ASTNode> condition;
    std::vector<std::unique_ptr<ASTNode>> body;
    WhileNode(std::unique_ptr<ASTNode> cond, std::vector<std::unique_ptr<ASTNode>> b)
        : condition(std::move(cond)), body(std::move(b)) {}
};

// узел AST для break
struct BreakNode : ASTNode {};

// узел AST для continue
struct ContinueNode : ASTNode {};

// узел AST для функциональных литералов
struct FunctionNode : ASTNode {
    std::vector<std::string> parameters;
    std::vector<std::unique_ptr<ASTNode>> body;
    FunctionNode(std::vector<std::string> params, std::vector<std::unique_ptr<ASTNode>> b)
        : parameters(std::move(params)), body(std::move(b)) {}
};

// узел AST для операторов return
struct ReturnNode : ASTNode {
    std::unique_ptr<ASTNode> expr;
    explicit ReturnNode(std::unique_ptr<ASTNode> e) : expr(std::move(e)) {}
};