#include <lexer.hpp>
#include <catch.hpp>
#include <string>
namespace Test{

    TEST_CASE("Lex simple example")
    {
        SECTION("Check with number")
        {
            StringSourceReader reader(std::string("5.2"));
            auto sut = Lexer<StringSourceReader>(reader);
    
            auto res = sut.tryReadNumber(std::optional('1'));
            REQUIRE(res.has_value());
            REQUIRE(std::holds_alternative<TokenNumber>(res.value()));
            auto expectedValue = 15.2;
            REQUIRE(std::get<TokenNumber>(res.value()).value == expectedValue);
        }

        SECTION("Check with identifier")
        {
            StringSourceReader reader(std::string("illeplus"));
            auto sut = Lexer<StringSourceReader>(reader);
    
            auto res = sut.tryReadIdentifier(std::optional('Z'));
            REQUIRE(res.has_value());
            REQUIRE(std::holds_alternative<TokenIdentifier>(res.value()));
            auto expected = std::string("Zilleplus");
            REQUIRE(std::get<TokenIdentifier>(res.value()).name == expected);
        }

        SECTION("Check special characters")
        {
            StringSourceReader reader(std::string(""));

            auto sut = Lexer<StringSourceReader>(reader);
    
            auto res0 = sut.tryReadCharacter(std::optional(',')).value();
            REQUIRE(std::holds_alternative<TokenComma>(res0));
            auto res1 = sut.tryReadCharacter(std::optional(')')).value();
            REQUIRE(std::holds_alternative<TokenCloseRBracket>(res1));
            auto res2 = sut.tryReadCharacter(std::optional('(')).value();
            REQUIRE(std::holds_alternative<TokenOpenRBracket>(res2));
        }

        SECTION("Check with comment")
        {
            StringSourceReader reader(std::string("blaaah blaaaah \n a"));
            auto sut = Lexer<StringSourceReader>(reader);
    
            auto res = sut.skipOverComments(std::optional('#'));

            auto expected = '\n';
            REQUIRE(res.has_value());
            REQUIRE(res.value() == expected);
        }

        auto testCode = std::string("\
        # Compute the x'th fibonacci number.\
        def fib(x)\
          if x < 3 then\
            1\
          else\
            fib(x-1)+fib(x-2)\
        \
        # This expression will compute the 40th number.\
        fib(40)"); 

        SECTION("Try simple example code")
        {
            StringSourceReader reader(testCode);
            auto sut = Lexer<StringSourceReader>(reader);
    
            std::vector<Token> results;
            while(true)
            {
                auto res = sut.getToken();
                if(res.has_value())
                {
                    results.push_back(res.value());
                }
                else{break;}
            }
        }
    }
}
