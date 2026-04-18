#include "codegen.h"

#include <stdexcept>
#include <cctype>
#include <unordered_map>

// ============================================================
//  IRInstruction
// ============================================================

std::string IRInstruction::toString() const {
    if (arg2.empty())
        return result + " = " + arg1;
    return result + " = " + arg1 + " " + op + " " + arg2;
}

// ============================================================
//  IRGenerator
// ============================================================

IRGenerator::IRGenerator() : tempCount(0) {}

std::string IRGenerator::newTemp() {
    return "t" + std::to_string(++tempCount);
}

std::string IRGenerator::generateExpr(const ASTNode* node) {
    if (const auto* num = dynamic_cast<const NumberNode*>(node))
        return std::to_string(num->value);

    if (const auto* var = dynamic_cast<const VariableNode*>(node))
        return var->name;

    if (const auto* binop = dynamic_cast<const BinaryOpNode*>(node)) {
        std::string lhs = generateExpr(binop->left.get());
        std::string rhs = generateExpr(binop->right.get());
        std::string tmp = newTemp();
        instructions.push_back({tmp, std::string(1, binop->op), lhs, rhs});
        return tmp;
    }

    throw std::runtime_error("IRGenerator: unhandled AST node type");
}

const std::vector<IRInstruction>& IRGenerator::generate(const ProgramNode& program) {
    instructions.clear();
    tempCount = 0;

    for (const auto& stmt : program.statements) {
        const auto* assign = dynamic_cast<const AssignmentNode*>(stmt.get());
        if (!assign)
            throw std::runtime_error("IRGenerator: expected AssignmentNode at top level");

        std::string exprResult = generateExpr(assign->expr.get());
        // Final "store" step: varName = exprResult
        instructions.push_back({assign->varName, "=", exprResult, ""});
    }

    return instructions;
}

// ============================================================
//  AsmInstruction
// ============================================================

std::string AsmInstruction::toString() const {
    if (src.empty())
        return "  " + opcode + " " + dst;
    return "  " + opcode + " " + dst + ", " + src;
}

// ============================================================
//  AsmGenerator
// ============================================================

std::string AsmGenerator::toOperand(const std::string& s) {
    if (!s.empty() &&
        (std::isdigit(static_cast<unsigned char>(s[0])) ||
         (s[0] == '-' && s.size() > 1)))
        return "#" + s;
    return s;
}

const std::vector<AsmInstruction>& AsmGenerator::generate(
    const std::vector<IRInstruction>& ir)
{
    instructions.clear();
    for (const auto& ins : ir) {
        if (ins.arg2.empty()) {
            // Simple copy: result = arg1
            instructions.push_back({"MOV", ins.result, toOperand(ins.arg1)});
        } else {
            // Binary operation
            static const std::unordered_map<std::string, std::string> opMap = {
                {"+", "ADD"}, {"-", "SUB"}, {"*", "MUL"}, {"/", "DIV"}
            };
            auto it = opMap.find(ins.op);
            if (it == opMap.end())
                throw std::runtime_error("AsmGenerator: unknown operator '" + ins.op + "'");
            instructions.emplace_back(AsmInstruction{"MOV",      "R1",       toOperand(ins.arg1)});
            instructions.emplace_back(AsmInstruction{it->second, "R1",       toOperand(ins.arg2)});
            instructions.emplace_back(AsmInstruction{"MOV",      ins.result, "R1"});
        }
    }
    return instructions;
}
