#include "semantic.h"

#include <stdexcept>

// ============================================================
//  SymbolTable
// ============================================================

void SymbolTable::declare(const std::string& name) {
    table[name] = 0;
}

bool SymbolTable::isDeclared(const std::string& name) const {
    return table.count(name) > 0;
}

void SymbolTable::set(const std::string& name, int value) {
    table[name] = value;
}

const std::unordered_map<std::string, int>& SymbolTable::all() const {
    return table;
}

// ============================================================
//  SemanticAnalyzer
// ============================================================

SemanticAnalyzer::SemanticAnalyzer(SymbolTable& symbolTable)
    : symTable(symbolTable) {}

// ----------------------------------------------------------------
// Internal tree-walk
// ----------------------------------------------------------------
void SemanticAnalyzer::checkNode(const ASTNode* node) {
    if (const auto* assign = dynamic_cast<const AssignmentNode*>(node)) {
        // LHS variable is being declared/assigned here
        symTable.declare(assign->varName);
        checkNode(assign->expr.get());

    } else if (const auto* binop = dynamic_cast<const BinaryOpNode*>(node)) {
        checkNode(binop->left.get());
        checkNode(binop->right.get());

    } else if (const auto* var = dynamic_cast<const VariableNode*>(node)) {
        if (!symTable.isDeclared(var->name))
            errors.push_back("Undefined variable '" + var->name + "'");

    }
    // NumberNode needs no checks
}

// ----------------------------------------------------------------
// Public entry point
// ----------------------------------------------------------------
void SemanticAnalyzer::analyze(const ProgramNode& program) {
    errors.clear();

    for (const auto& stmt : program.statements)
        checkNode(stmt.get());

    if (!errors.empty()) {
        std::string msg = "Semantic errors:\n";
        for (const auto& e : errors)
            msg += "  - " + e + "\n";
        throw std::runtime_error(msg);
    }
}
