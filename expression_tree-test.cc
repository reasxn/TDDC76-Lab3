/*
 * expression_tree-test.cc
 */
#include "Expression_Tree.h"
#include <iostream>
#include <stdexcept>
using namespace std;

int main()
{
   // Ska inte vara möjligt - Expression_Tree ska vara abstrakt:
   // Expression_Tree* t0{new Expression_Tree};

   Expression_Tree* t1{new Minus{new Real{11.14},new Plus{ new Real{3.14},new Integer{7}}}};
   //Expression_Tree* t2{new Minus{new Integer{7}, new Real{3.14}}};
   Expression_Tree* t3{new Times{new Integer{10}, new Real{3.14}}};
   Expression_Tree* t4{new Divide{new Integer{7}, new Real{3.14}}};
   Expression_Tree* t5{new Assign{new Variable{"x"},new Power{new Integer{10}, new Real{2.0}}}};
  
  t5->print(cout);
   

try
   {
      cout << "t1->evaluate() = " << t1->evaluate() << '\n';
      //cout << "t2->evaluate() = " << t2->evaluate() << '\n';
      cout << "t3->evaluate() = " << t3->evaluate() << '\n';
      cout << "t4->evaluate() = " << t4->evaluate() << '\n';
      cout << "t5->evaluate() = " << t5->evaluate() << '\n';
   }
   catch (const exception& e)
   {
      cout << "Undantag fångat: " << e.what() << '\n';
   }
 
cout << "t1->get_postfix() = " << t1->get_postfix() << '\n';
cout << "t1->str() = " << t1->str() << "\n\n";

Expression_Tree* t2{t5->clone()};

try
{
delete t1;
cout << "t2->evaluate() = " << endl;
t5->print(cout);
}
catch (const exception& e)
{
cout << "Undantag fångat: " << e.what() << '\n';
}
cout << "t2->get_postfix() = " << t2->get_postfix() << '\n';
cout << "t2->str() = " << t2->str() << "\n\n";

   return 0;
}
