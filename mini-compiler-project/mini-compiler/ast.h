#ifndef AST_H
#define AST_H

#include <memory>
#include <string>
#include <vector>

// ============================================================
//  AST Node Base & Concrete Types
// ============================================================

struct ASTNode {
    virtual ~ASTNode() = default;
    virtual std::string prettyPrint(const std::string& prefix = "",
                                    bool isLeft = false) const = 0;
};

using ASTNodePtr = std::unique_ptr<ASTNode>;

// ----------------------------------------------------------------
struct NumberNode : ASTNode {
    int value;
    explicit NumberNode(int v) : value(v) {}

    std::string prettyPrint(const std::string& prefix,
                            bool isLeft) const override;
};

// ----------------------------------------------------------------
struct VariableNode : ASTNode {
    std::string name;
    explicit VariableNode(std::string n) : name(std::move(n)) {}

    std::string prettyPrint(const std::string& prefix,
                            bool isLeft) const override;
};

// ----------------------------------------------------------------
struct BinaryOpNode : ASTNode {
    char op;
    ASTNodePtr left;
    ASTNodePtr right;

    BinaryOpNode(char o, ASTNodePtr l, ASTNodePtr r)
        : op(o), left(std::move(l)), right(std::move(r)) {}

    std::string prettyPrint(const std::string& prefix,
                            bool isLeft) const override;
};

// ----------------------------------------------------------------
struct AssignmentNode : ASTNode {
    std::string varName;
    ASTNodePtr expr;

    AssignmentNode(std::string v, ASTNodePtr e)
        : varName(std::move(v)), expr(std::move(e)) {}

    std::string prettyPrint(const std::string& prefix,
                            bool isLeft) const override;
};

// ----------------------------------------------------------------
struct ProgramNode : ASTNode {
    std::vector<ASTNodePtr> statements;

    std::string prettyPrint(const std::string& prefix,
                            bool isLeft) const override;
};

#endif  // AST_H
