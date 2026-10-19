
#ifndef CPPSCRIPT_LEXER_HPP
#define CPPSCRIPT_LEXER_HPP

#include "cppscript/token.hpp"

#include <string>
#include <vector>

namespace cppscript {

class Lexer {
public:
    Lexer(const std::string& source,
          const std::string& filename = "<memory>");

    std::vector<Token> tokenize();

private:
    const std::string source_;
    const std::string filename_;

    std::size_t position_ = 0;
    int line_ = 1;
    int column_ = 1;

    char currentChar() const;
    char peek(std::size_t offset = 1) const;
    bool isAtEnd() const;

    char advance();
    bool match(char expected);

    void skipWhitespace();
    void skipLineComment();
    void skipBlockComment();

    Token makeToken(TokenType type,
                    std::size_t start,
                    int startLine,
                    int startColumn) const;

    Token scanToken();
    Token scanIdentifierOrKeyword();
    Token scanNumber();
    Token scanString();
};

} // namespace cppscript

#endif
