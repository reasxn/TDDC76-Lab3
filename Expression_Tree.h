/*
 * Expression_Tree.h
 */
#ifndef EXPRESSIONTREE_H
#define EXPRESSIONTREE_H
#include <iosfwd>
#include <string>
#include <stdexcept>
#include "Variable_Table.h"
class expression_tree_error:public std::logic_error
{
 public:
  explicit expression_tree_error(const std::string& what_arg) noexcept
    : std::logic_error{what_arg} {}
	
  explicit expression_tree_error(const char* what_arg) noexcept
    : std::logic_error{what_arg} {}
};


class Expression_Tree
{
 public:
  virtual ~Expression_Tree() = default;
  virtual long double    evaluate(Variable_Table&) const = 0; 
  virtual std::string     get_postfix() const = 0;
  virtual std::string     str() const = 0;
  virtual void            print(std::ostream&) const = 0;
  virtual void			printHelper(std::ostream&, int) const = 0; 
  virtual Expression_Tree* clone() const = 0;
  virtual std::string 	get_infix() const = 0;
};

/*------Binary Operator--------*/

class Binary_Operator : public Expression_Tree
{
 public:
  ~Binary_Operator();
  void print(std::ostream&) const;
  std::string get_postfix() const;
  std::string get_infix() const;
 protected:
  Expression_Tree* left = nullptr;
  Expression_Tree* right= nullptr;
  void printHelper(std::ostream&, int) const; 
  std::string get_infixHelp(Expression_Tree*, Expression_Tree*) const;
};

/*----------Operand-----------*/
 
class Operand : public Expression_Tree
{	
 public:
  ~Operand() = default;
  void print(std::ostream&) const; 
  std::string get_postfix() const;
  std::string get_infix() const;
 protected:
  void printHelper(std::ostream&, int) const;
  };

/*-----------Plus-------------*/
 
class Plus : public Binary_Operator
{ 
 public:
  std::string str() const;
  Plus(Expression_Tree* l,Expression_Tree* r);
  Expression_Tree* clone() const;
  long double evaluate(Variable_Table&) const;
};

/*-----------Minus-------------*/

class Minus : public Binary_Operator 
{
 public:
  Expression_Tree* clone() const;
  std::string str() const;
  Minus(Expression_Tree* l,Expression_Tree* r);
  long double evaluate(Variable_Table&) const;
};

/*-----------Times-------------*/

class Times : public Binary_Operator
{
 public:
  Expression_Tree* clone() const;
  std::string str() const;
  Times(Expression_Tree* l,Expression_Tree* r);
  long double evaluate(Variable_Table&) const;
};


/*-----------Divide------------*/

class Divide : public Binary_Operator
{
 public:
  Expression_Tree* clone() const;
  std::string str() const;
  Divide(Expression_Tree* l,Expression_Tree* r);
  long double evaluate(Variable_Table&) const;

};

/*-----------Power-------------*/

class Power: public Binary_Operator
{
 public:
  Expression_Tree* clone() const;
  std::string str() const;
  Power(Expression_Tree* l,Expression_Tree* r);
  long double evaluate(Variable_Table&) const;
};

/*-----------Assign------------*/

class Assign: public Binary_Operator
{
 public:
  Expression_Tree* clone() const;
  std::string str() const;
  long double evaluate(Variable_Table&) const;
  Assign(Expression_Tree* l, Expression_Tree* r);
};

/*-----------Integer-----------*/

class Integer : public Operand
{
 public:
  Expression_Tree* clone() const;
  long long int value = 0;
  std::string str() const;
  Integer(long long int);
  long double evaluate(Variable_Table&) const;
};

/*-----------Real--------------*/

class Real : public Operand{
 public:
  long double value;
  Expression_Tree* clone() const;
  std::string str() const;
  Real(long double temp);
  long double evaluate(Variable_Table&) const;
};

/*-----------Variable-----------*/

class Variable : public Operand
{
 public:
  std::string name;
  long double evaluate(Variable_Table&) const;
  std::string str() const;
  long double value;
  Expression_Tree* clone() const;
  long double get_value() const;
  void set_value(const long double&);
  Variable(const std::string&);
};

#endif
