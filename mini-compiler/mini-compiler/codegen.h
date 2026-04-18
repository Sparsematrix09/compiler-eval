#ifndef CODEGEN_H
#define CODEGEN_H

#include "ast.h"

#include <string>
#include <vector>

// ============================================================
//  IR (Three-Address Code)
// ============================================================

struct IRInstruction {
    std::string result;
    std::string op;
    std::string arg1;
    std::string arg2;

    /// Human-readable form, e.g. "t1 = a + 3"
    std::string toString() const;
};

class IRGenerator {
public:
    IRGenerator();

    /// Traverse the AST and emit IR instructions.
    /// The returned vector is owned by this object.
    const std::vector<IRInstruction>& generate(const ProgramNode& program);

private:
    std::vector<IRInstruction> instructions;
    int                        tempCount;

    std::string newTemp();
    std::string generateExpr(const ASTNode* node);
};

// ============================================================
//  Pseudo-Assembly
// ============================================================

struct AsmInstruction {
    std::string opcode;
    std::string dst;
    std::string src; // empty for single-operand instructions

    /// Human-readable form, e.g. "  ADD R1, #3"
    std::string toString() const;
};

class AsmGenerator {
public:
    /// Lower IR instructions to pseudo-assembly.
    /// The returned vector is owned by this object.
    const std::vector<AsmInstruction>& generate(
        const std::vector<IRInstruction>& ir);

private:
    std::vector<AsmInstruction> instructions;

    /// Prefix numeric literals with '#'; leave register/variable names as-is.
    static std::string toOperand(const std::string& s);
};

#endif // CODEGEN_H
