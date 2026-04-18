#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "ast.h"

#include <string>
#include <vector>
#include <unordered_map>

// ============================================================
//  SymbolTable
// ============================================================

class SymbolTable {
public:
    /// Register a variable name (value defaults to 0).
    void declare(const std::string& name);

    /// Return true if the name has been declared.
    bool isDeclared(const std::string& name) const;

    /// Update the stored value for an already-declared variable.
    void set(const std::string& name, int value);

    /// Read-only access to the full table (name → value).
    const std::unordered_map<std::string, int>& all() const;

private:
    std::unordered_map<std::string, int> table;
};

// ============================================================
//  SemanticAnalyzer
// ============================================================

class SemanticAnalyzer {
public:
    /// The analyzer stores a reference to a caller-owned SymbolTable
    /// so the caller can inspect declared variables after analysis.
    explicit SemanticAnalyzer(SymbolTable& symbolTable);

    /// Walk the AST and enforce semantic rules.
    /// Throws std::runtime_error on the first batch of errors found.
    void analyze(const ProgramNode& program);

private:
    SymbolTable&             symTable;
    std::vector<std::string> errors;

    void checkNode(const ASTNode* node);
};

#endif // SEMANTIC_H
