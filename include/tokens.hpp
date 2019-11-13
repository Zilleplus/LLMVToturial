#ifndef TOKENS_H
#define TOKENS_H
#include <variant>
#include <type_traits>

// helper type for the visitor
template<class T> struct always_false : std::false_type {};

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

std::string token_to_string(Token t)
{
    return std::visit([](auto&& token)-> std::string {
            using T = std::decay_t<decltype(token)>;
            if constexpr (std::is_same_v<T, TokenEol>)
                return std::string("TokenEol"); 
            else if constexpr (std::is_same_v<T, TokenOpenRBracket>)
                return std::string("openBracket"); 
            else if constexpr (std::is_same_v<T, TokenCloseRBracket>)
                return std::string("closeBracket"); 
            else if constexpr (std::is_same_v<T, TokenComma>)
                return std::string("Comma"); 
            else if constexpr (std::is_same_v<T, TokenEof>)
                return std::string("Eof"); 
            else if constexpr (std::is_same_v<T, TokenDef>)
                return std::string("Def"); 
            else if constexpr (std::is_same_v<T, TokenExtern>)
                return std::string("Extern"); 
            else if constexpr (std::is_same_v<T, TokenIdentifier>)
            {
                auto identifier = std::get<TokenIdentifier>(token);
                return std::string("Identifier with name=")+identifier.name; 
            }
            else if constexpr (std::is_same_v<T, TokenNumber>)
                return std::string("Number"); 
            else 
                static_assert(always_false<T>::value, "non-exhaustive visitor!");
                return std::string();
        }, t);
}

#endif
