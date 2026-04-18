#include "parser.h"

#include <stdexcept>

// ============================================================
//  Constructor
// ============================================================

Parser::Parser(std::vector<Token> toks)
    : tokens(std::move(toks)), pos(0) {}

// ============================================================
//  Token navigation
// ============================================================

const Token& Parser::current() const {
    return tokens[pos];
}

const Token& Parser::peek(int offset) const {
    size_t idx = pos + static_cast<size_t>(offset);
    return idx < tokens.size() ? tokens[idx] : tokens.back();
}

Token Parser::consume(TokenType expected) {
    if (current().type != expected) {
        throw std::runtime_error(
            "Parse error at line " + std::to_string(current().line) +
            ", col "               + std::to_string(current().col)  +
            ": expected "          + tokenTypeName(expected)         +
            ", got "               + tokenTypeName(current().type)   +
            " ('"                  + current().value + "')");
    }
    return tokens[pos++];
}

// ============================================================
//  factor → NUMBER | IDENTIFIER | '(' expression ')'
// ============================================================

ASTNodePtr Parser::parseFactor() {
    if (current().type == TokenType::NUMBER) {
        int val = std::stoi(consume(TokenType::NUMBER).value);
        return std::make_unique<NumberNode>(val);
    }

    if (current().type == TokenType::IDENTIFIER) {
        std::string name = consume(TokenType::IDENTIFIER).value;
        return std::make_unique<VariableNode>(name);
    }

    if (current().type == TokenType::LPAREN) {
        consume(TokenType::LPAREN);
        ASTNodePtr expr = parseExpression();
        consume(TokenType::RPAREN);
        return expr;
    }

    throw std::runtime_error(
        "Parse error at line " + std::to_string(current().line) +
        ": unexpected token '" + current().value + "' in expression");
}

// ============================================================
//  term → factor (('*' | '/') factor)*
// ============================================================

ASTNodePtr Parser::parseTerm() {
    ASTNodePtr left = parseFactor();

    while (current().type == TokenType::STAR ||
           current().type == TokenType::SLASH) {
        char op = (current().type == TokenType::STAR) ? '*' : '/';
        ++pos;
        ASTNodePtr right = parseFactor();
        left = std::make_unique<BinaryOpNode>(op, std::move(left), std::move(right));
    }

    return left;
}

// ============================================================
//  expression → term (('+' | '-') term)*
// ============================================================

ASTNodePtr Parser::parseExpression() {
    ASTNodePtr left = parseTerm();

    while (current().type == TokenType::PLUS ||
           current().type == TokenType::MINUS) {
        char op = (current().type == TokenType::PLUS) ? '+' : '-';
        ++pos;
        ASTNodePtr right = parseTerm();
        left = std::make_unique<BinaryOpNode>(op, std::move(left), std::move(right));
    }

    return left;
}

// ============================================================
//  statement → IDENTIFIER '=' expression
// ============================================================

ASTNodePtr Parser::parseStatement() {
    if (current().type != TokenType::IDENTIFIER) {
        throw std::runtime_error(
            "Parse error at line " + std::to_string(current().line) +
            ": expected identifier at start of statement, got '" +
            current().value + "'");
    }

    std::string varName = consume(TokenType::IDENTIFIER).value;
    consume(TokenType::ASSIGN);
    ASTNodePtr expr = parseExpression();
    return std::make_unique<AssignmentNode>(varName, std::move(expr));
}

// ============================================================
//  parse → program
// ============================================================

std::unique_ptr<ProgramNode> Parser::parse() {
    auto program = std::make_unique<ProgramNode>();

    while (current().type != TokenType::END_OF_FILE) {
        program->statements.push_back(parseStatement());
        if (current().type == TokenType::SEMICOLON)
            ++pos; // consume optional semicolon
    }

    return program;
}
