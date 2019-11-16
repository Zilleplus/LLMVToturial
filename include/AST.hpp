#include<variant>
#include<string>
#include<memory>
#include<vector>

/// NumberExprAST - Expression class for numeric literals like "1.0".
struct NumberExprAST {
  const double val;

  NumberExprAST(double val) : val(val) {}
};

/// VariableExprAST - Expression class for referencing a variable, like "a".
struct VariableExprAST {
  const std::string name;

  VariableExprAST(const std::string &name) : name(name) {}
};

/// BinaryExprAST - Expression class for a binary operator.
template<typename T>
struct BinaryExprAST {
  const char op;
  const std::unique_ptr<T> LHS, RHS;

  BinaryExprAST(char op, std::unique_ptr<T> LHS,
                std::unique_ptr<T> RHS)
    : op(op), LHS(std::move(LHS)), RHS(std::move(RHS)) {}
};

/// CallExprAST - Expression class for function calls.
template<typename T>
struct CallExprAST {
  const std::string callee;
  const std::vector<std::unique_ptr<T>> args;

  CallExprAST(const std::string &callee,
              std::vector<std::unique_ptr<T>> args)
    : callee(callee), args(std::move(args)) {}
};

// -> recursively define the variant using fixed-point combinator
template<typename T>
using ExprASTNonRecursive = std::variant<
    NumberExprAST,
    VariableExprAST,
    BinaryExprAST<T>,
    CallExprAST<T>>;

// tie the knot
template<template<typename> typename K>
struct ExprASTTemplated : K<ExprASTTemplated<K>>
{
    using K<ExprASTTemplated> :: K;
};

using ExprAST = ExprASTTemplated<ExprASTNonRecursive>;

// PrototypeAST - This class represents the "prototype" for a function,
// which captures its name, and its argument names (thus implicitly the number
// of arguments the function takes).
struct PrototypeAST {
  const std::string name;
  const std::vector<std::string> args;

  PrototypeAST(const std::string &name, std::vector<std::string> args)
    : name(name), args(std::move(args)) {}
};

// FunctionAST - This class represents a function definition itself.
struct FunctionAST {
  const std::unique_ptr<PrototypeAST> proto;
  const std::unique_ptr<ExprAST> body;

  FunctionAST(std::unique_ptr<PrototypeAST> proto,
              std::unique_ptr<ExprAST> body)
    : proto(std::move(proto)), body(std::move(body)) {}
};
