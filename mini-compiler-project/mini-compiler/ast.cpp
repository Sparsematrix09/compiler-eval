#include "ast.h"

#include <string>

// ============================================================
//  NumberNode
// ============================================================

std::string NumberNode::prettyPrint(const std::string& prefix,
                                    bool isLeft) const {
    return prefix + (isLeft ? "+-- " : "\\-- ") + "NUM(" +
           std::to_string(value) + ")\n";
}

// ============================================================
//  VariableNode
// ============================================================

std::string VariableNode::prettyPrint(const std::string& prefix,
                                      bool isLeft) const {
    return prefix + (isLeft ? "+-- " : "\\-- ") + "VAR(" + name + ")\n";
}

// ============================================================
//  BinaryOpNode
// ============================================================

std::string BinaryOpNode::prettyPrint(const std::string& prefix,
                                      bool isLeft) const {
    std::string connector = isLeft ? "+-- " : "\\-- ";
    std::string childPrefix = prefix + (isLeft ? "|   " : "    ");

    std::string result = prefix + connector + "BINOP(" + op + ")\n";
    result += left->prettyPrint(childPrefix, true);
    result += right->prettyPrint(childPrefix, false);
    return result;
}

// ============================================================
//  AssignmentNode
// ============================================================

std::string AssignmentNode::prettyPrint(const std::string& prefix,
                                        bool isLeft) const {
    std::string connector = isLeft ? "+-- " : "\\-- ";
    std::string childPrefix = prefix + (isLeft ? "|   " : "    ");

    std::string result = prefix + connector + "ASSIGN(" + varName + ")\n";
    result += expr->prettyPrint(childPrefix, false);
    return result;
}

// ============================================================
//  ProgramNode
// ============================================================

std::string ProgramNode::prettyPrint(const std::string& /*prefix*/,
                                     bool /*isLeft*/) const {
    std::string result = "PROGRAM\n";
    for (size_t i = 0; i < statements.size(); ++i) {
        bool isLeftChild = (i + 1 < statements.size());
        result += statements[i]->prettyPrint("", isLeftChild);
    }
    return result;
}