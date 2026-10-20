#include "cppscript/lexer.hpp"

#include <cctype>
#include <utility>

namespace cppscript {

Lexer::Lexer(const std::string& source, const std::string& filename)
    : source_(source), filename_(filename) {}

bool Lexer::isAtEnd() const {
    return position_ >= source_.size();
}

char Lexer::currentChar() const {
    return isAtEnd() ? '\0' : source_[position_];
}

char Lexer::peek(std::size_t offset) const {
    const std::size_t index = position_ + offset;
    return index >= source_.size() ? '\0' : source_[index];
}

char Lexer::advance() {
    if (isAtEnd()) {
        return '\0';
    }

    const char c = source_[position_++];

    if (c == '\n') {
        ++line_;
        column_ = 1;
    } else {
        ++column_;
    }

    return c;
}

bool Lexer::match(char expected) {
    if (isAtEnd() || currentChar() != expected) {
        return false;
    }

    advance();
    return true;
}

void Lexer::skipWhitespace() {
    while (!isAtEnd() &&
           std::isspace(static_cast<unsigned char>(currentChar()))) {
        advance();
    }
}

void Lexer::skipLineComment() {
    while (!isAtEnd() && currentChar() != '\n') {
        advance();
    }
}

void Lexer::skipBlockComment() {
    advance(); // /
    advance(); // *

    while (!isAtEnd()) {
        if (currentChar() == '*' && peek() == '/') {
            advance();
            advance();
            return;
        }

        advance();
    }
}

Token Lexer::makeToken(TokenType type,
                       std::size_t start,
                       int startLine,
                       int startColumn) const {
    return {
        type,
        source_.substr(start, position_ - start),
        {filename_, startLine, startColumn}
    };
}

Token Lexer::scanIdentifierOrKeyword() {
    const std::size_t start = position_;
    const int startLine = line_;
    const int startColumn = column_;

    while (std::isalnum(static_cast<unsigned char>(currentChar())) ||
           currentChar() == '_') {
        advance();
    }

    const std::string word = source_.substr(start, position_ - start);

    const auto keyword = [&word]() -> TokenType {
        if (word == "show") return TokenType::Show;
        if (word == "state") return TokenType::State;
        if (word == "int") return TokenType::Int;
        if (word == "let") return TokenType::Let;
        if (word == "const") return TokenType::Const;
        if (word == "bring") return TokenType::Bring;

        if (word == "if") return TokenType::If;
        if (word == "else") return TokenType::Else;
        if (word == "switch") return TokenType::Switch;
        if (word == "case") return TokenType::Case;
        if (word == "default") return TokenType::Default;

        if (word == "for") return TokenType::For;
        if (word == "while") return TokenType::While;
        if (word == "do") return TokenType::Do;
        if (word == "break") return TokenType::Break;
        if (word == "continue") return TokenType::Continue;

        if (word == "class") return TokenType::Class;
        if (word == "public") return TokenType::Public;
        if (word == "private") return TokenType::Private;
        if (word == "protected") return TokenType::Protected;
        if (word == "new") return TokenType::New;
        if (word == "this") return TokenType::This;

        if (word == "try") return TokenType::Try;
        if (word == "catch") return TokenType::Catch;
        if (word == "finally") return TokenType::Finally;
        if (word == "throw") return TokenType::Throw;

        if (word == "import") return TokenType::Import;
        if (word == "export") return TokenType::Export;

        if (word == "true") return TokenType::True;
        if (word == "false") return TokenType::False;
        if (word == "null") return TokenType::Null;

        return TokenType::Identifier;
    };

    return makeToken(keyword(), start, startLine, startColumn);
}

Token Lexer::scanNumber() {
    const std::size_t start = position_;
    const int startLine = line_;
    const int startColumn = column_;

    while (std::isdigit(static_cast<unsigned char>(currentChar()))) {
        advance();
    }

    TokenType type = TokenType::Integer;

    if (currentChar() == '.' &&
        std::isdigit(static_cast<unsigned char>(peek()))) {
        type = TokenType::Decimal;
        advance();

        while (std::isdigit(static_cast<unsigned char>(currentChar()))) {
            advance();
        }
    }

    return makeToken(type, start, startLine, startColumn);
}

Token Lexer::scanString() {
    const std::size_t start = position_;
    const int startLine = line_;
    const int startColumn = column_;

    const char quote = advance();

    while (!isAtEnd()) {
        if (currentChar() == quote) {
            advance();
            return makeToken(TokenType::String,
                             start, startLine, startColumn);
        }

        if (currentChar() == '\n') {
            advance();
            return makeToken(TokenType::Invalid,
                             start, startLine, startColumn);
        }

        if (currentChar() == '\\') {
            advance();

            if (isAtEnd()) {
                break;
            }

            advance();
        } else {
            advance();
        }
    }

    return makeToken(TokenType::Invalid, start, startLine, startColumn);
}

Token Lexer::scanToken() {
    skipWhitespace();

    const std::size_t start = position_;
    const int startLine = line_;
    const int startColumn = column_;

    if (isAtEnd()) {
        return makeToken(TokenType::EndOfFile,
                         start, startLine, startColumn);
    }

    const char c = currentChar();

    if (std::isalpha(static_cast<unsigned char>(c)) || c == '_') {
        return scanIdentifierOrKeyword();
    }

    if (std::isdigit(static_cast<unsigned char>(c))) {
        return scanNumber();
    }

    if (c == '"' || c == '\'') {
        return scanString();
    }

    advance();

    switch (c) {
        case '/':
            if (currentChar() == '/') {
                skipLineComment();
                return scanToken();
            }
            if (currentChar() == '*') {
                --position_;
                skipBlockComment();
                return scanToken();
            }
            if (match('=')) return makeToken(TokenType::SlashAssign, start, startLine, startColumn);
            return makeToken(TokenType::Slash, start, startLine, startColumn);

        case '+':
            if (match('+')) return makeToken(TokenType::Increment, start, startLine, startColumn);
            if (match('=')) return makeToken(TokenType::PlusAssign, start, startLine, startColumn);
            return makeToken(TokenType::Plus, start, startLine, startColumn);

        case '-':
            if (match('-')) return makeToken(TokenType::Decrement, start, startLine, startColumn);
            if (match('=')) return makeToken(TokenType::MinusAssign, start, startLine, startColumn);
            return makeToken(TokenType::Minus, start, startLine, startColumn);

        case '*':
            if (match('=')) return makeToken(TokenType::StarAssign, start, startLine, startColumn);
            return makeToken(TokenType::Star, start, startLine, startColumn);

        case '%': return makeToken(TokenType::Percent, start, startLine, startColumn);
        case '(': return makeToken(TokenType::LeftParen, start, startLine, startColumn);
        case ')': return makeToken(TokenType::RightParen, start, startLine, startColumn);
        case '{': return makeToken(TokenType::LeftBrace, start, startLine, startColumn);
        case '}': return makeToken(TokenType::RightBrace, start, startLine, startColumn);
        case '[': return makeToken(TokenType::LeftBracket, start, startLine, startColumn);
        case ']': return makeToken(TokenType::RightBracket, start, startLine, startColumn);
        case ',': return makeToken(TokenType::Comma, start, startLine, startColumn);
        case '.': return makeToken(TokenType::Dot, start, startLine, startColumn);
        case ':': return makeToken(TokenType::Colon, start, startLine, startColumn);
        case ';': return makeToken(TokenType::Semicolon, start, startLine, startColumn);

        case '=':
            if (match('>')) return makeToken(TokenType::Arrow, start, startLine, startColumn);
            if (match('=')) return makeToken(TokenType::Equal, start, startLine, startColumn);
            return makeToken(TokenType::Assign, start, startLine, startColumn);

        case '!':
            if (match('=')) return makeToken(TokenType::NotEqual, start, startLine, startColumn);
            return makeToken(TokenType::LogicalNot, start, startLine, startColumn);

        case '<':
            if (match('=')) return makeToken(TokenType::LessEqual, start, startLine, startColumn);
            return makeToken(TokenType::Less, start, startLine, startColumn);

        case '>':
            if (match('=')) return makeToken(TokenType::GreaterEqual, start, startLine, startColumn);
            return makeToken(TokenType::Greater, start, startLine, startColumn);

        case '&':
            if (match('&')) return makeToken(TokenType::LogicalAnd, start, startLine, startColumn);
            break;

        case '|':
            if (match('|')) return makeToken(TokenType::LogicalOr, start, startLine, startColumn);
            break;
    }

    return makeToken(TokenType::Invalid, start, startLine, startColumn);
}

std::vector<Token> Lexer::tokenize() {
    std::vector<Token> tokens;

    while (true) {
        Token token = scanToken();
        tokens.push_back(token);

        if (token.type == TokenType::EndOfFile) {
            break;
        }
    }

    return tokens;
}

} // namespace cppscript