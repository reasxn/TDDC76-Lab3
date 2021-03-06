/*
 * Calculator.h
 */
#ifndef CALCULATOR_H
#define CALCULATOR_H
#include "Variable_Table.h"
#include "Expression.h"
#include <vector>
#include <iosfwd>

/**
 * Calculator: klass f�r att l�sa in, lagra och bearbeta enkla aritmetiska
 * uttryck. F�r att anv�nda kalylatorn - skapa ett Calculator-objekt och 
 * anropa run().
 *
 * Anm: move-konstruktorn och move-illdelningsoperatorn genereras inte om
 * en destruktor, kopieringskonstruktor eller kopieringstilldelnings-
 * operator deklareras.
 */
class Calculator
{
 public:
  Calculator() = default;
  ~Calculator() = default;

  void run();

 private:
  Variable_Table vtab; 
  Calculator(const Calculator&) = delete;
  Calculator& operator=(const Calculator&) = delete;
  // Move-konstruktor och move-tilldelning genereras inte.

  static const std::string valid_cmds_;

  Expression current_expression_;
  std::vector<Expression> ExpressionVector;
  char command_;
  unsigned int exprNum;
   

  static void print_help();

  void get_command();
  bool valid_command() const;
  void execute_command();

  void read_expression(std::istream&);
};

#endif
