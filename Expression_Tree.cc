/*
 * Expression_Tree.cc
 */
#include "Expression_Tree.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <stdexcept>
#include "Variable_Table.h"

using namespace std;




/*------Binary Operator--------*/
Binary_Operator::Binary_Operator(Expression_Tree* lhs, Expression_Tree* rhs){
  left = lhs;
  right = rhs;
}

Binary_Operator::~Binary_Operator(){
  delete left;
  delete right;
}

std::string Binary_Operator::get_infixHelp(bool paranthesis) const{
  string expression = left->get_infixHelp(priorityCheck(left)) + " " + this->str() + 
    " " + right->get_infixHelp(priorityCheck(right));
	
  if(paranthesis){
    return "(" + expression + ")";
  }
  return expression;
}

bool Binary_Operator::priorityCheck(Expression_Tree* ptr) const {
  /* Minus is somewhat of a special case, this solution gives some redundant paranthesis
     but ensures mathematical correctness. 
  */
  if(this->str() == "-"){ 
    if(this->priority() >= ptr->priority()){ 
	
      return true;
    }
    return false;
  }
  if(this->priority() > ptr->priority()){
    return true;
  }
  return false;
	
}
	
void Binary_Operator::printHelper(std::ostream& os,int offset) const{
  //Print right side first.
	
  right->printHelper(os,offset + 2);
	
  //Printing this stuff
  os << setw(offset + 2) << "/" << endl;
  os << setw(offset) << str() << endl;
  os << setw(offset + 2) << "\\" << endl; 

  //Print Left
  left->printHelper(os, (offset + 2));
}

void Binary_Operator::print(std::ostream& os) const{
  printHelper(os, 0);
}

std::string Binary_Operator::get_postfix() const{
  return left->get_postfix() + " " + right->get_postfix() + " " + str();
}

std::string Binary_Operator::get_infix() const{
  return this->get_infixHelp(false);
}

/*----------Operand-----------*/

std::string Operand::get_infixHelp(bool) const{
  return str();
}

unsigned int Operand::priority() const{
  return 0;
}

void Operand::printHelper(std::ostream& os,int offset) const{ //Helps adjust the print
  offset += str().size();
  os << setw(offset) << str() << endl;
}

void Operand::print(std::ostream& os) const {
  printHelper(os, 0);
}

std::string Operand::get_postfix() const {
  return str();
}

std::string Operand::get_infix() const {
  return str();
}
/*-----------Plus-------------*/

unsigned int Plus::priority() const{
  return 0;
}

Expression_Tree* Plus::clone() const{

  Expression_Tree* templeft = nullptr;
  Expression_Tree* tempright =  nullptr; 
  try{
    Expression_Tree* templeft = left->clone();
    Expression_Tree* tempright = right->clone(); 
    return new Plus{templeft,tempright};} //To avoid memory problems
  catch(...){
    delete templeft;
    delete tempright;
    throw;
  }
}

string Plus::str() const{
  return "+";
}

long double Plus::evaluate(Variable_Table& ptr) const{
  return left->evaluate(ptr) + right->evaluate(ptr); 
}

/*-----------Minus-------------*/

unsigned int Minus::priority() const{
  return 1;
}

Expression_Tree* Minus::clone() const{
  Expression_Tree* templeft = nullptr;
  Expression_Tree* tempright =  nullptr; 
  try{
    Expression_Tree* templeft = left->clone();
    Expression_Tree* tempright = right->clone(); 
    return new Minus{templeft,tempright};} 
  catch(...){
    delete templeft;
    delete tempright;
    throw;
  }
}


string Minus::str() const{
  return "-";
}

long double Minus::evaluate(Variable_Table& ptr) const{
  return left->evaluate(ptr) - right->evaluate(ptr);
}

/*-----------Times-------------*/

unsigned int Times::priority() const{
  return 2;
}


Expression_Tree* Times::clone() const{
  Expression_Tree* templeft = nullptr;
  Expression_Tree* tempright =  nullptr; 
  try{
    Expression_Tree* templeft = left->clone();
    Expression_Tree* tempright = right->clone(); 
    return new Times{templeft,tempright};}
  catch(...){
    delete templeft;
    delete tempright;
    throw;
  }
}

string Times::str() const{
  return "*";
}

long double Times::evaluate(Variable_Table& ptr) const{
  return left->evaluate(ptr) * right->evaluate(ptr);
}


/*-----------Divide------------*/

unsigned int Divide::priority() const{
  return 2;
}

Expression_Tree* Divide::clone() const{
  Expression_Tree* templeft = nullptr;
  Expression_Tree* tempright =  nullptr; 
  try{
    Expression_Tree* templeft = left->clone();
    Expression_Tree* tempright = right->clone(); 
    return new Divide{templeft,tempright};}
  catch(...){
    delete templeft;
    delete tempright;
    throw;
  }
}

string Divide::str() const{
  return "/";
}

long double Divide::evaluate(Variable_Table& ptr) const{
  if(right->evaluate(ptr) == 0){
    throw expression_tree_error("Division by 0");
  }
  return left->evaluate(ptr) / right->evaluate(ptr);
}

/*-----------Power-------------*/

unsigned int Power::priority() const{
  return 3;
}

Expression_Tree* Power::clone() const{
  Expression_Tree* templeft = nullptr;
  Expression_Tree* tempright =  nullptr; 
  try{
    Expression_Tree* templeft = left->clone();
    Expression_Tree* tempright = right->clone(); 
    return new Power{templeft,tempright};}
  catch(...){
    delete templeft;
    delete tempright;
    throw;
  }
}

string Power::str() const{
  return "^";
}

long double Power::evaluate(Variable_Table& ptr) const{
  long double leftEval = left->evaluate(ptr);
  long double rightEval = right->evaluate(ptr);
  if(leftEval < 0 && floor(rightEval) != rightEval){
    throw expression_tree_error("Negative number raised by non-integer.");
  }
  return pow(leftEval,rightEval);
}

/*-----------Assign------------*/

unsigned int Assign::priority() const{
  return 0;
}

string Assign::str() const{
  return "="; 
}

Expression_Tree* Assign::clone() const{
  Expression_Tree* templeft = nullptr;
  Expression_Tree* tempright =  nullptr; 
  try{
    Expression_Tree* templeft = left->clone();
    Expression_Tree* tempright = right->clone(); 
    return new Assign{templeft,tempright};}
  catch(...){
    delete templeft;
    delete tempright;
    throw;
  }
}

long double Assign::evaluate(Variable_Table& ptr) const {
  Variable* temp = dynamic_cast<Variable*>(left);
  if (not(temp)){
    throw expression_tree_error("No variable left of assign");
  }
  temp->set_value(right->evaluate(ptr),ptr);
  return temp->get_value(ptr);
}

/*-----------Integer-----------*/

Expression_Tree* Integer::clone() const{
  return new Integer{value};
}

Integer::Integer(long long int temp){
  value = temp;
}

string Integer::str() const{
  return to_string(value);
}

long double Integer::evaluate(Variable_Table&) const {
  return value;
}

/*-----------Real--------------*/
Expression_Tree* Real::clone() const{
  return new Real{value};
}

Real::Real(long double temp){
  value = temp;
}

string Real::str() const{
  return to_string(value);
}

long double Real::evaluate(Variable_Table&) const{
  return value; 
}

/*-----------Variable-----------*/

Expression_Tree* Variable::clone() const{
  return new Variable{name};  
}

long double Variable::evaluate(Variable_Table& ptr) const {
  return ptr.get_value(this->str());
}

long double Variable::get_value(Variable_Table& ptr) const{
  return ptr.get_value(this->str());
}

void Variable::set_value(const long double& inputValue,Variable_Table& ptr){
  if(ptr.find(this->str())){
    ptr.set_value(this->str(),inputValue);
    return;
  }
  ptr.insert(this->str(),inputValue);
  return;
}

string Variable::str() const{
  return name;
}

Variable::Variable(const std::string& inputName) {
  name = inputName;
}
