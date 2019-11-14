#include <ctype.h>
#include <string>
#include <variant>
#include <optional>
#include <map>
#include <assert.h>
#include <tokens.hpp>

class StringSourceReader
{
    public:
        StringSourceReader(std::string source):
            source(std::move(source)){}

        std::optional<char> consumeNextCharacter()
        {
            if(currentIndex >= source.length()){return {};}
            return std::optional<char>{source.at(currentIndex++)};
        }

        std::optional<char> peekNextChar()
        {
            if(currentIndex >= source.length()){return {};}
            return std::optional<char>{source.at(currentIndex)};
        }
    private:
        int currentIndex = 0;
        std::string source;
};

template<typename TSourceReader>
class Lexer
{
    std::map<char,Token> specialCharaterTokens =
    {
        {'(',TokenOpenRBracket()},
        {')',TokenCloseRBracket()},
        {',',TokenComma()},
        {'\n',TokenEol()}
    };

public:
    Lexer(TSourceReader reader): reader(reader){}

    std::optional<Token> tryReadCharacter(std::optional<char> c)
    {
        if(c.has_value())
        {
            auto sc = specialCharaterTokens.find(c.value());
            return (sc == specialCharaterTokens.end())
                ? std::optional<Token>{}
                : sc->second;
        }

        return {};
    }

    std::optional<Token> tryReadIdentifier(std::optional<char> c)
    {
        if (c.has_value() && isalpha(c.value())) 
        { // identifier: [a-za-Z][a-zA-Z0-9]*
            auto identifierStr = std::string("")+ c.value();

            while ( reader.peekNextChar().has_value() 
                    && isalnum(reader.peekNextChar().value()))
            {
                auto newChar = reader.consumeNextCharacter();
                assert(newChar.has_value());
                identifierStr += newChar.value();
            }
            
             if (identifierStr == "def")
               return Token(TokenDef());
             if (identifierStr == "extern")
               return Token(TokenExtern());

            return Token(TokenIdentifier(std::move(identifierStr)));
        }
        return {};
    }

    std::optional<Token> tryReadNumber(std::optional<char> c)
    {
        if (c.has_value() &&(isdigit(c.value()) || c.value() == '.'))
        {   // Number: [0-9.]+
            std::string numStr; 
            numStr+=c.value();

            while (reader.peekNextChar().has_value() 
                  && (isdigit(reader.peekNextChar().value()) 
                  || reader.peekNextChar().value() == '.'))
            {
                numStr += reader.consumeNextCharacter().value();
            } 
            return Token(TokenNumber(strtod(numStr.c_str(), 0)));
        }
        return {};
    }
    
    // comments get ignored -> so void
    std::optional<char> skipOverComments(std::optional<char> c)
    {
        if (c.has_value() && c.value() == '#') 
        {
            // Comment until end of line.
            do c = reader.consumeNextCharacter();
            while ( c.has_value() 
                    && (c.value() != EOF) 
                    && (c.value() != '\n'));
        }
        return c;
    }

    // also skip over return, we already record \n which is more then enough.
    std::optional<char> skipOverWhiteSpace(std::optional<char> c)
    {
        while (c.has_value() && (isspace(c.value()) || (c.value()=='\r')))
            c = reader.consumeNextCharacter();
        return c;
    }

    // gettok - Return the next token from standard input.
    std::optional<Token>  getToken() 
    {
        std::optional<char> lastChar = reader.consumeNextCharacter();

        lastChar = skipOverWhiteSpace(lastChar);
        lastChar = skipOverComments(lastChar);

        auto number = tryReadNumber(lastChar);
        if(number.has_value()){return number.value();}
    
        auto identifier = tryReadIdentifier(lastChar);
        if(identifier.has_value()){return identifier.value();}

        auto specialCharacter = tryReadCharacter(lastChar);
        if(specialCharacter.has_value()){return specialCharacter.value();}

        // Check for end of file.    Don't eat the EOF.
        if (lastChar.has_value() && lastChar.value() == EOF)
            return Token(TokenEof());
    
        return {}; // empty line !
    }
private:
    TSourceReader reader;
};
