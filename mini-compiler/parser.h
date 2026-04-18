#ifndef PARSER_H
#define PARSER_H

/*
 * Recursive-descent parser for the grammar:
 *
 *   program        → statement* EOF
 *   statement      → IDENTIFIER '=' expression ';'?
 *   expression     → term   (('+' | '-') term)*
 *   term           → factor (('*' | '/') factor)*
 *   factor         → NUMBER | IDENTIFIER | '(' expression ')'
 */

#include "lexer.h"
#include "ast.h"

#include <vector>
#include <memory>

class Parser {
public:
    explicit Parser(std::vector<Token> tokens);

    /// Parse the token stream and return the program AST.
    std::unique_ptr<ProgramNode> parse();

private:
    std::vector<Token> tokens;
    size_t             pos;

    // ---- Token navigation ----
    const Token& current()              const;
    const Token& peek(int offset = 0)  const;
    Token        consume(TokenType expected);

    // ---- Grammar rules ----
    ASTNodePtr parseStatement();
    ASTNodePtr parseExpression();
    ASTNodePtr parseTerm();
    ASTNodePtr parseFactor();
};

#endif // PARSER_H
