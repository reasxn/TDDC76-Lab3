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
Binary_Operator::~Binary_Operator(){
delete left;
delete right;
}

//Helps decide where to put the parentheses
std::string Binary_Operator::get_infixHelp(Expression_Tree* left,Expression_Tree* right) const{
	std::string tempLeft, tempRight;
	
	tempLeft = left->get_infix();
	tempRight = right->get_infix();
	Expression_Tree* temp = this->clone();
	if(dynamic_cast<Times*>(temp) || (dynamic_cast<Divide*>(temp))
		 || (dynamic_cast<Power*>(temp))) {
		if(not(dynamic_cast<Operand*>(left))) {
			tempLeft =  "(" + left->get_infix() + ")";
			}
			else{
			tempLeft =  left->get_infix();
			}
			}
		if(not(dynamic_cast<Operand*>( right))) {
			tempRight = "(" +  right->get_infix() + ")";
			}
			else{
			tempRight =  right->get_infix();
			}
	if(dynamic_cast<Minus*>(temp)) {
			tempLeft =  left->get_infix();
		if(not(dynamic_cast<Operand*>( right))) {
			tempRight = "(" +  right->get_infix() + ")";
			}
			else{
			tempRight =  right->get_infix();
			}
			}
	if(dynamic_cast<Assign*>(temp)) {
			tempLeft =  left->get_infix();
			tempRight =  right->get_infix();
			}
		return tempLeft +  str() + tempRight;
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
	return this->get_infixHelp(this->left,this->right);
}

/*----------Operand-----------*/

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
Plus::Plus(Expression_Tree* l, Expression_Tree* r){
	left = l; right = r;
}

Expression_Tree* Plus::clone() const{

Expression_Tree* templeft = left->clone();
Expression_Tree* tempright = right->clone(); 
try{
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
Minus::Minus(Expression_Tree* l, Expression_Tree* r){
	left=l; right = r;
}

Expression_Tree* Minus::clone() const{
Expression_Tree* templeft = left->clone();
Expression_Tree* tempright = right->clone();
try{ 
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
Times::Times(Expression_Tree* l, Expression_Tree* r){
	left=l; right = r;
}

Expression_Tree* Times::clone() const{
Expression_Tree* templeft = left->clone();
Expression_Tree* tempright = right->clone(); 
try{
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
Divide::Divide(Expression_Tree* l, Expression_Tree* r){
	left=l; right = r;
}

Expression_Tree* Divide::clone() const{
Expression_Tree* templeft = left->clone();
Expression_Tree* tempright = right->clone(); 
try{
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
Power::Power(Expression_Tree* l, Expression_Tree* r){
	left=l; right = r;
}

Expression_Tree* Power::clone() const{
Expression_Tree* templeft = left->clone();
Expression_Tree* tempright = right->clone(); 
try{
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
	return pow(left->evaluate(ptr),right->evaluate(ptr));
}

/*-----------Assign------------*/

string Assign::str() const{
	return "="; 
}

Expression_Tree* Assign::clone() const{
Expression_Tree* templeft = left->clone();
Expression_Tree* tempright = right->clone();
try{ 
return new Assign{templeft,tempright};}
catch(...){
delete templeft;
delete tempright;
throw;
}
}

Assign::Assign(Expression_Tree* l, Expression_Tree* r){
	Variable* temp = dynamic_cast<Variable*>(l);
	if (not(temp)){
	throw expression_tree_error("No variable left of assign");
	}
	left = temp;
	right = r;
}

long double Assign::evaluate(Variable_Table& ptr) const {
	ptr.set_value(left->str(),right->evaluate(ptr)); 
	return ptr.get_value(left->str());
}

/*-----------Integer-----------*/

Expression_Tree* Integer::clone() const{
try{
return new Integer{value};
}
catch(...){
throw;
}
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
try{
return new Real{value};
}
catch(...){
throw;
}
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
try{
return new Variable{name};
}
catch(...){
throw;
}
}

long double Variable::evaluate(Variable_Table& ptr) const {
	return ptr.get_value(this->str());
}

long double Variable::get_value() const{
	return value;
}

void Variable::set_value(const long double& inputValue){
	value=inputValue;
}

string Variable::str() const{
	return name;
}

Variable::Variable(const std::string& inputName) {
	name = inputName;
}