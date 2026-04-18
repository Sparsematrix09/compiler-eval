#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>

// ============================================================
//  Token
// ============================================================

enum class TokenType {
    IDENTIFIER,
    NUMBER,
    PLUS,
    MINUS,
    STAR,
    SLASH,
    ASSIGN,
    SEMICOLON,
    LPAREN,
    RPAREN,
    END_OF_FILE
};

std::string tokenTypeName(TokenType t);

struct Token {
    TokenType type;
    std::string value;
    int line;
    int col;

    Token(TokenType t, std::string v, int l, int c)
        : type(t), value(std::move(v)), line(l), col(c) {}
};

// ============================================================
//  Lexer
// ============================================================

class Lexer {
   public:
    explicit Lexer(std::string source);

    /// Consume the entire source and return the token stream.
    std::vector<Token> tokenize();

   private:
    std::string src;
    size_t pos;
    int line;
    int col;

    char current() const;
    char advance();
    void skipWhitespace();

    Token readNumber();
    Token readIdentifier();
};

#endif  // LEXER_H
