#include <lexer.hpp>
#include <string>

// -------------------------
// Read out the example code:
// -------------------------
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

TEST_CASE("Lex simple example")
{
    SCENARIO("Check if all tokens are in output")
    {
    }
}
