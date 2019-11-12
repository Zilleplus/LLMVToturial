#ifndef PARSER_H
#define PARSER_H
#include<memory>
#include<string>
#include<AST.hpp>
#include<lexer.hpp>

class Parser
{
    public:
        Parser(Lexer lexer,Logger logger): lexer(lexer),logger(logger){}

        std::unique_ptr<ExprAST> parse()
        {
            Token currentToken;
            do{
                currentToken = lexer.getToken();
            }
            while(currentToken!=tok_eof)

            return null;
        }
    private:
        Lexer lexer;
        Logger logger;

        static std::unique_ptr<ExprAST> ParseNumberExpr() 
        {
            auto Result = std::make_unique<NumberExprAST>(NumVal);
            getNextToken(); // consume the number
            return std::move(Result);
        }        

        static std::unique_ptr<ExprAST> ParseParenExpr()
        {
            lexer.getToken(); // eat (.
            auto V = ParseExpression();
            if (!V)
              return nullptr;

            if (CurTok != ')')
              return LogError("expected ')'");
            getNextToken(); // eat ).
            return V;
        }
};

#endif
