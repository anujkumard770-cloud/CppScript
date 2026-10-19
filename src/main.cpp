
// #include <iostream>

// int main() {
//     std::cout << "CppScript compiler development started!\n";
//     std::cout << "Language version: 0.1.0\n";

//     return 0;
// }




#include "cppscript/lexer.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

const char* tokenTypeName(cppscript::TokenType type) {
    using cppscript::TokenType;

    switch (type) {
        case TokenType::EndOfFile: return "EOF";
        case TokenType::Invalid: return "INVALID";
        case TokenType::Identifier: return "IDENTIFIER";
        case TokenType::Integer: return "INTEGER";
        case TokenType::Decimal: return "DECIMAL";
        case TokenType::String: return "STRING";

        case TokenType::Show: return "SHOW";
        case TokenType::State: return "STATE";
        case TokenType::Int: return "INT";
        case TokenType::Let: return "LET";
        case TokenType::Const: return "CONST";
        case TokenType::Bring: return "BRING";

        case TokenType::If: return "IF";
        case TokenType::Else: return "ELSE";
        case TokenType::Switch: return "SWITCH";
        case TokenType::Case: return "CASE";
        case TokenType::Default: return "DEFAULT";

        case TokenType::For: return "FOR";
        case TokenType::While: return "WHILE";
        case TokenType::Do: return "DO";
        case TokenType::Break: return "BREAK";
        case TokenType::Continue: return "CONTINUE";

        case TokenType::Class: return "CLASS";
        case TokenType::Public: return "PUBLIC";
        case TokenType::Private: return "PRIVATE";
        case TokenType::Protected: return "PROTECTED";
        case TokenType::New: return "NEW";
        case TokenType::This: return "THIS";

        case TokenType::Try: return "TRY";
        case TokenType::Catch: return "CATCH";
        case TokenType::Finally: return "FINALLY";
        case TokenType::Throw: return "THROW";
        case TokenType::Import: return "IMPORT";
        case TokenType::Export: return "EXPORT";
        case TokenType::True: return "TRUE";
        case TokenType::False: return "FALSE";
        case TokenType::Null: return "NULL";

        case TokenType::Plus: return "PLUS";
        case TokenType::Minus: return "MINUS";
        case TokenType::Star: return "STAR";
        case TokenType::Slash: return "SLASH";
        case TokenType::Percent: return "PERCENT";
        case TokenType::Assign: return "ASSIGN";
        case TokenType::Equal: return "EQUAL";
        case TokenType::NotEqual: return "NOT_EQUAL";
        case TokenType::Less: return "LESS";
        case TokenType::LessEqual: return "LESS_EQUAL";
        case TokenType::Greater: return "GREATER";
        case TokenType::GreaterEqual: return "GREATER_EQUAL";
        case TokenType::LogicalAnd: return "LOGICAL_AND";
        case TokenType::LogicalOr: return "LOGICAL_OR";
        case TokenType::LogicalNot: return "LOGICAL_NOT";
        case TokenType::PlusAssign: return "PLUS_ASSIGN";
        case TokenType::MinusAssign: return "MINUS_ASSIGN";
        case TokenType::StarAssign: return "STAR_ASSIGN";
        case TokenType::SlashAssign: return "SLASH_ASSIGN";
        case TokenType::Arrow: return "ARROW";
        case TokenType::Increment: return "INCREMENT";
        case TokenType::Decrement: return "DECREMENT";

        case TokenType::LeftParen: return "LEFT_PAREN";
        case TokenType::RightParen: return "RIGHT_PAREN";
        case TokenType::LeftBrace: return "LEFT_BRACE";
        case TokenType::RightBrace: return "RIGHT_BRACE";
        case TokenType::LeftBracket: return "LEFT_BRACKET";
        case TokenType::RightBracket: return "RIGHT_BRACKET";
        case TokenType::Comma: return "COMMA";
        case TokenType::Dot: return "DOT";
        case TokenType::Colon: return "COLON";
        case TokenType::Semicolon: return "SEMICOLON";
    }

    return "UNKNOWN";
}

int main(int argc, char* argv[]) {
    const std::string filename =
        argc > 1 ? argv[1] : "examples/hello.cps";

    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Error: Cannot open source file: "
                  << filename << '\n';
        return 1;
    }

    std::ostringstream buffer;
    buffer << file.rdbuf();

    cppscript::Lexer lexer(buffer.str(), filename);
    const auto tokens = lexer.tokenize();

    std::cout << "CppScript Lexer v0.1.0\n";
    std::cout << "Source: " << filename << "\n\n";

    bool hasInvalidToken = false;

    for (const auto& token : tokens) {
        std::cout << token.location.line << ':'
                  << token.location.column << "  "
                  << tokenTypeName(token.type) << "  "
                  << '"' << token.lexeme << '"' << '\n';

        if (token.type == cppscript::TokenType::Invalid) {
            hasInvalidToken = true;
        }
    }

    return hasInvalidToken ? 1 : 0;
}
