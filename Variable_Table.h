/*
 * Calculator.h
 */
#ifndef VARIABLETABLE_H
#define VARIABLETABLE_H
#include <iostream>
#include <map>
#include <utility>
#include <string>

/**
 * Calculator: klass för att läsa in, lagra och bearbeta enkla aritmetiska
 * uttryck. För att använda kalylatorn - skapa ett Calculator-objekt och 
 * anropa run().
 *
 * Anm: move-konstruktorn och move-illdelningsoperatorn genereras inte om
 * en destruktor, kopieringskonstruktor eller kopieringstilldelnings-
 * operator deklareras.
 */
 
class vt_error:public std::logic_error
{
 public:
  explicit vt_error(const std::string& what_arg) noexcept
    : std::logic_error{what_arg} {}
	
  explicit vt_error(const char* what_arg) noexcept
    : std::logic_error{what_arg} {}
};

 
class Variable_Table
{
 public:
  Variable_Table() = default;
  ~Variable_Table() = default;
  std::map<std::string,long double> VarTable;
  void insert(std::string,long double);
  void remove(std::string);
  bool find(const std::string) const;
  void set_value(std::string,long double);
  long double get_value(const std::string) const;
  void list(std::ostream&);
  void clear();
  bool empty() const;
};  
#endif
