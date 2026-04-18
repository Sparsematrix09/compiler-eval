#include "lexer.h"

#include <cctype>
#include <stdexcept>

// ============================================================
//  tokenTypeName
// ============================================================

std::string tokenTypeName(TokenType t) {
    switch (t) {
        case TokenType::IDENTIFIER:  return "IDENTIFIER";
        case TokenType::NUMBER:      return "NUMBER";
        case TokenType::PLUS:        return "PLUS";
        case TokenType::MINUS:       return "MINUS";
        case TokenType::STAR:        return "STAR";
        case TokenType::SLASH:       return "SLASH";
        case TokenType::ASSIGN:      return "ASSIGN";
        case TokenType::SEMICOLON:   return "SEMICOLON";
        case TokenType::LPAREN:      return "LPAREN";
        case TokenType::RPAREN:      return "RPAREN";
        case TokenType::END_OF_FILE: return "EOF";
    }
    return "UNKNOWN";
}

// ============================================================
//  Lexer – constructor
// ============================================================

Lexer::Lexer(std::string source)
    : src(std::move(source)), pos(0), line(1), col(1) {}

// ============================================================
//  Private helpers
// ============================================================

char Lexer::current() const {
    return pos < src.size() ? src[pos] : '\0';
}

char Lexer::advance() {
    char c = src[pos++];
    if (c == '\n') { ++line; col = 1; }
    else            { ++col; }
    return c;
}

void Lexer::skipWhitespace() {
    while (pos < src.size() && std::isspace(static_cast<unsigned char>(current())))
        advance();
}

Token Lexer::readNumber() {
    int startCol = col;
    std::string num;
    while (pos < src.size() && std::isdigit(static_cast<unsigned char>(current())))
        num += advance();
    return Token(TokenType::NUMBER, num, line, startCol);
}

Token Lexer::readIdentifier() {
    int startCol = col;
    std::string ident;
    while (pos < src.size() &&
           (std::isalnum(static_cast<unsigned char>(current())) || current() == '_'))
        ident += advance();
    return Token(TokenType::IDENTIFIER, ident, line, startCol);
}

// ============================================================
//  tokenize
// ============================================================

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (true) {
        skipWhitespace();

        if (pos >= src.size()) {
            tokens.emplace_back(TokenType::END_OF_FILE, "EOF", line, col);
            break;
        }

        char c        = current();
        int  startCol = col;

        if (std::isdigit(static_cast<unsigned char>(c))) {
            tokens.push_back(readNumber());
            continue;
        }
        if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
            tokens.push_back(readIdentifier());
            continue;
        }

        advance(); // consume the single-char operator
        switch (c) {
            case '+': tokens.emplace_back(TokenType::PLUS,      "+", line, startCol); break;
            case '-': tokens.emplace_back(TokenType::MINUS,     "-", line, startCol); break;
            case '*': tokens.emplace_back(TokenType::STAR,      "*", line, startCol); break;
            case '/': tokens.emplace_back(TokenType::SLASH,     "/", line, startCol); break;
            case '=': tokens.emplace_back(TokenType::ASSIGN,    "=", line, startCol); break;
            case ';': tokens.emplace_back(TokenType::SEMICOLON, ";", line, startCol); break;
            case '(': tokens.emplace_back(TokenType::LPAREN,    "(", line, startCol); break;
            case ')': tokens.emplace_back(TokenType::RPAREN,    ")", line, startCol); break;
            default:
                throw std::runtime_error(
                    "Unknown character '" + std::string(1, c) +
                    "' at line " + std::to_string(line) +
                    ", col "  + std::to_string(startCol));
        }
    }
    return tokens;
}
