#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include "codegen.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

// ============================================================
//  CompilerDriver  –  orchestrates all phases and pretty-printing
// ============================================================

class CompilerDriver {
public:
    void compile(const std::string& source) {
        std::cout << "\n>>> Source: " << source << "\n";

        // ── Phase 1: Lexical analysis ────────────────────────
        Lexer lexer(source);
        auto tokens = lexer.tokenize();
        printTokens(tokens);

        // ── Phase 2: Parsing ─────────────────────────────────
        Parser parser(tokens);
        auto program = parser.parse();
        printAST(*program);

        // ── Phase 3a: Semantic analysis ──────────────────────
        printBanner("PHASE 3a: SEMANTIC ANALYSIS");
        SymbolTable      symTable;
        SemanticAnalyzer analyzer(symTable);
        analyzer.analyze(*program);

        std::cout << "  No semantic errors. Declared variables:\n";
        for (const auto& entry : symTable.all())
            std::cout << "    - " << entry.first << "\n";

        // ── Phase 3b: IR generation ───────────────────────────
        IRGenerator irGen;
        const auto& ir = irGen.generate(*program);
        printIR(ir);

        // ── Phase 4: Assembly generation ─────────────────────
        AsmGenerator asmGen;
        const auto& asmCode = asmGen.generate(ir);
        printAsm(asmCode);

        std::cout << "\n[Compilation successful]\n";
    }

private:
    // ---- Display helpers ----

    static void printBanner(const std::string& title) {
        const std::string line(50, '=');
        std::cout << "\n" << line << "\n"
                  << "  " << title << "\n"
                  << line << "\n";
    }

    static void printTokens(const std::vector<Token>& tokens) {
        printBanner("PHASE 1: TOKENS");
        std::cout << std::left
                  << std::setw(16) << "TYPE"
                  << std::setw(16) << "VALUE"
                  << std::setw(8)  << "LINE"
                  << "COL\n"
                  << std::string(44, '-') << "\n";

        for (const auto& t : tokens) {
            if (t.type == TokenType::END_OF_FILE) continue;
            std::cout << std::left
                      << std::setw(16) << tokenTypeName(t.type)
                      << std::setw(16) << t.value
                      << std::setw(8)  << t.line
                      << t.col << "\n";
        }
    }

    static void printAST(const ProgramNode& program) {
        printBanner("PHASE 2: ABSTRACT SYNTAX TREE");
        std::cout << program.prettyPrint("", false);
    }

    static void printIR(const std::vector<IRInstruction>& ir) {
        printBanner("PHASE 3: THREE-ADDRESS INTERMEDIATE CODE");
        for (const auto& ins : ir)
            std::cout << "  " << ins.toString() << "\n";
    }

    static void printAsm(const std::vector<AsmInstruction>& asmCode) {
        printBanner("PHASE 4: PSEUDO-ASSEMBLY OUTPUT");
        for (const auto& ins : asmCode)
            std::cout << ins.toString() << "\n";
    }
};

// ============================================================
//  main
// ============================================================

int main() {
    // Tie cerr to cout so both streams flush together in the same order.
    // This prevents stdout/stderr interleaving on Windows terminals.
    std::ios::sync_with_stdio(true);
    std::cerr.tie(&std::cout);

    CompilerDriver driver;

    const std::vector<std::string> testCases = {
        "a = 5 + 3;",
        "a = 5 + 3; b = a * 2;",
        "x = (10 - 4) * 3; y = x / 2 + 1;",
        "result = 100 / (4 + 1) * 2 - 7;"
    };

    for (const auto& src : testCases) {
        try {
            driver.compile(src);
        } catch (const std::exception& e) {
            std::cout.flush();
            std::cerr << "\n[ERROR] " << e.what() << "\n";
            std::cerr.flush();
        }
        std::cout << "\n" << std::string(60, '#') << "\n";
    }

    // Demonstrate error detection
    std::cout << "\n Testing error case: use of undeclared variable\n";
    try {
        driver.compile("z = w + 1;");
    } catch (const std::exception& e) {
        std::cout.flush();
        std::cerr << "\n[CAUGHT EXPECTED ERROR] " << e.what() << "\n";
        std::cerr.flush();
    }

    return 0;
}