/*
 * Expression.h
 */
#ifndef EXPRESSION_H
#define EXPRESSION_H
#include <iosfwd>
#include <stdexcept>
#include <string>
#include "Variable_Table.h"

/**
 * expression_error: kastas om fel inträffar i en Expression- operation;
 * ett diagnostiskt meddelande ska skickas med.
 */

class expression_error: public std::logic_error
{
 public:
  explicit expression_error(const std::string& what_arg) noexcept
    : std::logic_error{what_arg} {}
	
  explicit expression_error(const char* what_arg) noexcept
    : std::logic_error{what_arg} {}
};

/**
 * Expression: Klass för att representera ett enkelt aritmetiskt uttryck.
 */
class Expression
{
 public:
  // OBSERVERA: DETTA ÄR ENDAST KODSKELETT - MODIFIERA OCH KOMPLETTERA!
  
  Variable_Table *vtab = nullptr;
  Expression(class Expression_Tree*);
  Expression() = default;
  ~Expression();
   
  Expression(const Expression&);	//Kopieringskonstruktor
  Expression(const Expression&&); 	//Movesemantics
   
  Expression& operator=(const Expression&);	//Assignment operator
  Expression& operator=(const Expression&&); //The 5th of the 5th that was missing.  <- Ska skrivas
   
  void 		  clear();
  long double evaluate(Variable_Table&) const;
  std::string get_postfix() const;
  std::string get_infix() const;
  bool        empty() const;
  void        print_tree(std::ostream&) const;
  void        swap(Expression&);
 private:
  class Expression_Tree *topnode = nullptr;
};

/**
 * swap: Byter innehåll på två Expression-objekt.
 */
void swap(Expression&, Expression&);

/**
 * make_expression: Hjälpfunktion för att skapa ett Expression-objekt, givet
 * ett infixuttryck i form av en sträng.
 */
Expression make_expression(const std::string& infix);

#endif
