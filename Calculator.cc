/*
 * Calculator.cc
 */
#include "Calculator.h"
#include "Expression.h"
#include <cctype>
#include <iostream>
#include <stdexcept>
#include <string>
using namespace std;

const string Calculator::valid_cmds_("?HUBPTSILANRVX");

/**
 * run: Huvudfunktionen f५r kalkylatorn. Skriver ut hjॊlpinformation
 * och lॊser sedan sedan in ett kommando i taget och utf५r det.
 */
void
Calculator::
run()
{
  cout << "Vॊlkommen till Kalkylatorn!\n\n";
  print_help();

  do 
    {
      try 
	{
	  get_command();
	  if (valid_command()) execute_command();
	}
      catch (const exception& e) 
	{
	  cout << e.what() << '\n';
	}
      // Of५rutsedda undantag avbryter programk५rningen, via main().
    }
  while (command_ != 'S');
}

/**
 * print_help: Skriver ut kommandorepertoaren.
 */
void
Calculator::
print_help()
{
  cout << "  H, ?  Skriv ut denna information\n";
  cout << "  U     Mata in ett nytt uttryck\n";
  cout << "  B     Berॊkna aktuellt uttryck\n";
  cout << "  B  n  Ber�kna uttryck n\n";
  cout << "  P     Visa aktuellt uttryck som postfix\n";
  cout << "  P  n  Visa uttryck n som postfix\n";
  cout << "  I     Visa aktuellt uttryck som infix\n";
  cout << "  I  n  Visa uttryck n som infix\n";
  cout << "  L     Lista alla uttryck som infix\n";
  cout << "  T     Visa aktuellt uttryck som ett tr�d\n";
  cout << "  T  n  Visa uttryck n som ett tr�d\n";
  cout << "  N     Visa antal lagrade uttryck\n";
  cout << "  A  n  G�r uttryck n till aktuellt uttryck\n";
  cout << "  R     Radera aktuellt uttryck\n";
  cout << "  R  n  Radera uttryck n\n";
  cout << "  V     Lista alla variabler\n";
  cout << "  X     Radera alla variabler\n";
  cout << "  S     Avsluta kalkylatorn\n";
}

/**
 * get_command: Lॊser ett kommando (ett tecken), g५r om till versal och lagrar
 * kommandot i medlemmen command_, f५r vidare behandling av andra operationer. 
 * Ingen kontroll g५rs om det skrivits mer, i sो fall skrॊp, pो kommandoraden.
 */
void
Calculator::
get_command()
{
  cout << ">> ";
  cin >> command_;
  while (cin.peek() == ' '){
    cin.ignore();
  }
  if(isdigit(cin.peek())){
    cin >> exprNum;
  }
  else exprNum = 0;
  command_ = toupper(command_);
}

/**
 * valid_command: Kontrollerar om kommandot som finns i medlemmen command_
 * tillh५r den tillोtna kommandorepertoraren och returnerar antingen true
 * (giltigt kommando) eller false (ogiltigt kommando).
 */
bool
Calculator::
valid_command() const
{
  if (valid_cmds_.find(command_) == string::npos)
    {
      cout << "Otillोtet kommando: " << command_ << "\n";
      return false;
    }
  return true;
}

/**
 * execute_command: Utf५r kommandot som finns i medlemmen command_. Kommandot
 * f५rutsॊtts ha kontrollerats med valid_command() och alltsो ॊr ett giltigt 
 * kommando.
 */
void
Calculator::
execute_command()
{
  if (command_ == 'H' || command_ == '?')
    print_help();
  else if (command_ == 'U')
    read_expression(cin);
  else if (command_ == 'B' && exprNum != 0)
    cout << ExpressionVector.at(exprNum-1).evaluate(vtab) << "\n";
  else if (command_ == 'B')
    cout << current_expression_.evaluate(vtab) << "\n";
  else if (command_ == 'P' && exprNum != 0)
    cout << ExpressionVector.at(exprNum-1).get_postfix() << "\n";
  else if (command_ == 'P')
    cout << current_expression_.get_postfix() << "\n";
  else if (command_ == 'I' && exprNum !=0)
    cout << ExpressionVector.at(exprNum-1).get_infix() << "\n";
  else if (command_ == 'I')
    cout << current_expression_.get_infix() << "\n";
  else if (command_ == 'L')
    for(unsigned int i = 0; i < ExpressionVector.size(); i++){
      cout << i+1 << ": " << ExpressionVector.at(i).get_infix() << "\n";
    }
  else if (command_ == 'T' && exprNum !=0){
    ExpressionVector.at(exprNum-1).print_tree(cout);
    cout << "\n";
  }
  else if (command_ == 'T')
    current_expression_.print_tree(cout);
    
  else if (command_ == 'N')
    cout << ExpressionVector.size() << "\n";
  else if (command_ == 'A' && exprNum !=0){
    current_expression_ = ExpressionVector.at(exprNum -1 );
    ExpressionVector.insert(ExpressionVector.begin(), current_expression_);
    ExpressionVector.erase(ExpressionVector.begin() + exprNum);
  }
  else if (command_ == 'R' && exprNum !=0){
    if(ExpressionVector.size() < exprNum){
      cout << "Trying to delete, non-existing expression" << endl;
      return;
    }
    ExpressionVector.erase(ExpressionVector.begin()+exprNum-1);
    current_expression_.clear();
    if(ExpressionVector.size() > 0){
      current_expression_ = ExpressionVector.at(0);
    }}
  else if (command_ == 'R'){
    if(ExpressionVector.size() >0){
      current_expression_.clear(); 
      ExpressionVector.erase(ExpressionVector.begin());
      if(ExpressionVector.size() > 0){
      	current_expression_ = ExpressionVector.at(0);
      }}}
  else if (command_ == 'V')
    vtab.list(cout);
  else if (command_ == 'X')
    vtab.clear();
  else if (command_ == 'S')
    cout << "Kalkylatorn avlutas, vॊlkommen ोter!\n";
  else
    cout << "Detta ska inte hॊnda!\n";
}

/**
 * read_expression: lॊser ett infixuttryck frोn instr५mmen is och ger detta 
 * till funktionen make_expression() som returnerar ett objekt av typen 
 * Expression, vilket lagras som "aktuellt uttryck" i current_expression_.
 */
void
Calculator::
read_expression(istream& is)
{
  string infix;

  is >> ws;

  if (getline(is, infix))
    {
      current_expression_ = make_expression(infix);
      ExpressionVector.insert(ExpressionVector.begin(),current_expression_);
    }
  else
    {
      cout << "Felaktig inmatning!\n";
    }
}
