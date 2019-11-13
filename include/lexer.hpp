#include <ctype.h>
#include <string>
#include <variant>
#include <optional>
#include <map>
#include <tokens.hpp>

class StringSourceReader
{
    public:
        StringSourceReader(std::string source):
            source(std::move(source)){}

        std::optional<char> getNextChar()
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
        { // identifier: [a-zA-Z][a-zA-Z0-9]*
            auto identifierStr = std::string("");
            do
            {
                identifierStr += c.value();
                c = reader.getNextChar();
            }
            while (c.has_value() && isalnum(c.value()));
            
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
          std::string NumStr; 
          do {
            NumStr += c.value();
            c = reader.getNextChar();
          } while (c.has_value() 
                  && (isdigit(c.value()) 
                  || c.value() == '.'));
        
          return Token(TokenNumber(strtod(NumStr.c_str(), 0)));
        }
        return {};
    }
    
    // comments get ignored -> so void
    std::optional<char> skipOverComments(std::optional<char> c)
    {
        if (c.has_value() && c.value() == '#') 
        {
            // Comment until end of line.
            do c = reader.getNextChar();
            while ( c.has_value() 
                    && (c.value() != EOF) 
                    && (c.value() != '\n')
                    && (c.value() != '\r'));
        }
        return c;
    }

    // also skip over return, we already record \n which is more then enough.
    std::optional<char> skipOverWhiteSpace(std::optional<char> c)
    {
        while (c.has_value() && (isspace(c.value()) || (c.value()=='\r')))
            c = reader.getNextChar();
        return c;
    }

    // gettok - Return the next token from standard input.
    std::optional<Token>  getToken() 
    {
        std::optional<char> lastChar = reader.getNextChar();

        lastChar = skipOverWhiteSpace(lastChar);
        lastChar = skipOverComments(lastChar);
        lastChar = skipOverWhiteSpace(lastChar);

        if(!lastChar.has_value()){return {};}

        auto number = tryReadNumber(lastChar);
        if(number.has_value()){return number.value();}
    
        auto identifier = tryReadIdentifier(lastChar);
        if(identifier.has_value()){return identifier.value();}

        auto specialCharacter = tryReadCharacter(lastChar);
        if(specialCharacter.has_value()){return specialCharacter.value();}

        // Check for end of file.    Don't eat the EOF.
        if (lastChar.value() == EOF)
            return Token(TokenEof());
    
        return {}; // empty line !
    }
private:
    TSourceReader reader;
};
