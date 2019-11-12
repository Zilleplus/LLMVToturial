#ifndef TOKENS_H
#define TOKENS_H
#include <variant>

struct TokenEol{}; // end of line aka \n -> \r's are ignored
struct TokenOpenRBracket{}; // (
struct TokenCloseRBracket{}; // )
struct TokenComma{}; // ,
struct TokenEof{};
struct TokenDef{};
struct TokenExtern{};
struct TokenIdentifier
{
    TokenIdentifier(std::string name) : name(std::move(name)){}
    const std::string name;
};
struct TokenNumber
{
    TokenNumber(double value) : value(value){}
    const double value;
};

using Token = std::variant<
    TokenEol, // end of line aka \n -> \r's are ignored
    TokenOpenRBracket, // (
    TokenCloseRBracket, // )
    TokenComma, // ,
    TokenEof,
    TokenDef,
    TokenExtern,
    TokenIdentifier,
    TokenNumber>;

#endif
