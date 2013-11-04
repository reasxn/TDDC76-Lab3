/*
 * Calculator.cc
 */
#include "Variable_Table.h"


using namespace std;

void Variable_Table::insert(std::string name,long double value){
VarTable.insert(pair<string,long double>(name,value));
}

void Variable_Table::remove(string str){
if(VarTable.find(str) != VarTable.end()){
	VarTable.erase(VarTable.find(str));
}
}

bool Variable_Table::find(string name) const{
return VarTable.count(name);
}
	
	
void Variable_Table::set_value(string name,long double value){
if(find(name)){
	VarTable.at(name) = value;
	}
insert(name,value);
}

long double Variable_Table::get_value(string name) const{
if(find(name)){
	return VarTable.at(name);
	}
	cout << "Variable not found, 0 is used" << endl; 
	return 0;
}

void Variable_Table::list(ostream& os){
for(auto it = VarTable.cbegin(); it != VarTable.cend(); ++it){
os << it->first<<": " << it->second <<endl; 
}
}

void Variable_Table::clear(){
VarTable.clear();
}
bool Variable_Table::empty() const{
return VarTable.empty();
}

