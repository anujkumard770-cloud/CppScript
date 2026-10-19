
#ifndef CPPSCRIPT_TOKEN_HPP
#define CPPSCRIPT_TOKEN_HPP

#include <string>

namespace cppscript {

enum class TokenType {
    // Special
    EndOfFile,
    Invalid,

    // Literals and identifiers
    Identifier,
    Integer,
    Decimal,
    String,

    // Keywords
    Show,
    State,
    Int,
    Let,
    Const,
    Bring,

    If,
    Else,
    Switch,
    Case,
    Default,

    For,
    While,
    Do,
    Break,
    Continue,

    Class,
    Public,
    Private,
    Protected,
    New,
    This,

    Try,
    Catch,
    Finally,
    Throw,

    Import,
    Export,

    True,
    False,
    Null,

    // Operators
    Plus,
    Minus,
    Star,
    Slash,
    Percent,

    Assign,
    Equal,
    NotEqual,
    Less,
    LessEqual,
    Greater,
    GreaterEqual,

    LogicalAnd,
    LogicalOr,
    LogicalNot,

    PlusAssign,
    MinusAssign,
    StarAssign,
    SlashAssign,

    Arrow,
    Increment,
    Decrement,

    // Punctuation
    LeftParen,
    RightParen,
    LeftBrace,
    RightBrace,
    LeftBracket,
    RightBracket,

    Comma,
    Dot,
    Colon,
    Semicolon
};

struct SourceLocation {
    std::string file;
    int line = 1;
    int column = 1;
};

struct Token {
    TokenType type;
    std::string lexeme;
    SourceLocation location;
};

} // namespace cppscript

#endif
