/*
 * Expression.cc
 */
#include "Expression.h"
#include "Variable_Table.h"
#include "Expression_Tree.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>
using namespace std;

/*
 * Class functions Yay!
 */
Expression::Expression(class Expression_Tree* temp) {
    topnode = temp;
}

long double Expression::evaluate(Variable_Table& vtab) const
{
    if(empty()){
        throw expression_error("Trying to evaluate an empty expression.");
    }
    return topnode->evaluate(vtab);
}

Expression::~Expression(){
    delete this->topnode;
    this->topnode = nullptr;
}


/*
 **	Copyconstructor
 **/

Expression::Expression(const Expression& other){
    if(other.empty()){
        this->topnode = nullptr;
        return;
    }
    this->topnode = other.topnode->clone();
}

/*
 * Constructors
 */

Expression::Expression(Expression&& other){
    swap(other);										//Modified, using swap instead
}

Expression& Expression::operator=(Expression&& other)   //Added
{
    swap(other);
    return *this;
}

Expression& Expression::operator=(const Expression& other){
    if(this == &other){
        return *this;
    }
    if(other.empty()){
        this->topnode = nullptr; //Is defined as defaul in h, but just to be sure.
        return *this;
    }
    this->topnode = other.topnode->clone();
    return *this;
}

void Expression::clear(){
    delete topnode;
    topnode = nullptr;	//Deleted the object itself, now it deletes the topnode ptr, then
    //sets that value to nullptr.
}

/*
 * get_postfix()
 */
std::string Expression::get_postfix() const
{
    if(empty()){
        return "Empty expression";
    }
    return topnode->get_postfix();
}

std::string Expression::get_infix() const {
    if(empty()){
        return "Empty expression";
    }
    return topnode->get_infix();
}

/*
 * empty()
 */
bool Expression::empty() const
{
    if(topnode == nullptr){
        return true;
    }
    return false;
}

/*
 * print_tree()
 */
void Expression::print_tree(std::ostream& os) const
{
    if(empty()){
        cout << "The tree you are trying to print is empty" << endl;
        return; //To better indicate what happens to the user.
    }
    topnode->print(os);
    return;
}

/*
 * swap(other)
 */
void Expression::swap(Expression& other)
{
    if(this == &other){
        return;
    }
    Expression_Tree *temp1 = this->topnode;
    this->topnode = other.topnode;
    other.topnode = temp1;
    return;
}

/*
 * swap(x, y)
 */
void swap(Expression& lhs, Expression& rhs)
{
    lhs.swap(rhs);
}

/*
 * make_expression() definieras efter namnrymden nedan.
 */
Expression make_expression(const string& infix);

// Namrymden nedan innehोller intern kod f५r infix-till-postfix-omvandling
// och generering av uttryckstrॊd. En anonym namnrymd begrॊnsar anvॊndningen
// av medlemmarna till denna fil.
namespace
{
    using std::vector;
    using std::map;
    using std::make_pair;
    using std::string;
    
    // Teckenuppsॊttningar f५r operander. Anvॊnds av make_expression_tree().
    const string letters{"abcdefghijklmnopqrstuvwxyz"};
    const string digits{"0123456789"};
    const string integer_chars{digits};
    const string real_chars{digits + '.'};
    const string variable_chars{letters};
    const string operand_chars{letters + digits + '.'};
    
    // Tillोtna operatorer. Anvॊnds av make_postfix() och make_expression_tree().
    // Prioritetstabeller, en f५r inkommandeprioritet och en f५r stackprioritet.
    // H५gre vॊrde inom input_prio respektive stack_prio anger inb५rdes prioritetsordning.
    // H५gre vॊrde i input_prio jॊmf५rt med motsvarande position i stack_prio innebॊr
    // h५gerassociativitet, det motsatta vॊnsterassociativitet. Anvॊnds av make_postfix().
    using priority_table = map<string, int>;
    
    const vector<string> operators{ "^", "*", "/", "+", "-", "=" };
    const priority_table input_priority{ {"^", 8}, {"*", 5}, {"/", 5}, {"+", 3}, {"-", 3}, {"=", 2} };
    const priority_table stack_priority{ {"^", 7}, {"*", 6}, {"/", 6}, {"+", 4}, {"-", 4}, {"=", 1} };
    
    // Hjॊlpfunktioner f५r att kategorisera lexikala element.
    bool is_operator(char token)
    {
        return find(begin(operators), end(operators), string{token}) != end(operators);
    }
    
    bool is_operator(const string& token)
    {
        return find(begin(operators), end(operators), token) != end(operators);
    }
    
    bool is_operand(const string& token)
    {
        return token.find_first_not_of(operand_chars) == string::npos;
    }
    
    bool is_integer(const string& token)
    {
        return token.find_first_not_of(integer_chars) == string::npos;
    }
    
    bool is_real(const string& token)
    {
        return token.find_first_not_of(real_chars) == string::npos;
    }
    
    bool is_identifier(const string& token)
    {
        return token.find_first_not_of(letters) == string::npos;
    }
    
    // format_infix tar en strॊng med ett infixuttryck och formaterar med ett
    // mellanrum mellan varje symbol; underlॊttar vid bearbetningen i make_postfix.
    std::string format_infix(const std::string& infix)
    {
        auto bos = begin(infix);
        auto eos = end(infix);
        string formated;
        
        for (auto it = bos; it != eos; ++it)
        {
            if (is_operator(*it) || *it == '(' || *it == ')')
            {
                // Se till att det ॊr ett mellanrum f५re en operator eller parentes
                if (it != bos && *(it - 1) != ' ' && *(formated.end() - 1) != ' ')
                    formated.append(1, ' ');
                formated.append(1, *it);
                // Se till att det ॊr ett mellanrum efter en operator eller parentes
                if ((it + 1) != eos && *(it + 1) != ' ')
                    formated.append(1, ' ');
            }
            else
            {
                if (*it != ' ')
                    formated.append(1, *it);
                else if (it != bos && *(it - 1) != ' ')
                    formated.append(1, *it);
            }
        }
        return formated;
    }
    
    // make_postfix tar en infixstrॊng och returnerar motsvarande postfixstrॊng.
    std::string make_postfix(const std::string& infix)
    {
        using std::stack;
        using std::string;
        using std::istringstream;
        using std::find;
        
        stack<string> operator_stack;
        string        token;
        string        previous_token;
        bool          last_was_operand{false};
        bool          assignment{false};
        int           paren_count{0};
        
        istringstream is{format_infix(infix)};
        string        postfix;
        
        while (is >> token)
        {
            if (is_operator(token))
            {
                if (!last_was_operand || postfix.empty() || previous_token == "(")
                {
                    throw expression_error("operator dr operand frvntades\n");
                }
                
                if (token == "=")
                {
                    if (assignment)
                    {
                        throw expression_error("multipel tilldelning");
                    }
                    else
                    {
                        assignment = true;
                    }
                }
                
                while (!operator_stack.empty() && operator_stack.top() != "(" &&
                       input_priority.find(token)->second <=
                       stack_priority.find(operator_stack.top())->second)
                {
                    postfix += operator_stack.top() + ' ';
                    operator_stack.pop();
                }
                operator_stack.push(token);
                last_was_operand = false;
            }
            else if (token == "(")
            {
                operator_stack.push(token);
                ++paren_count;
            }
            else if (token == ")")
            {
                if (paren_count == 0)
                {
                    throw expression_error("vnsterparentes saknas\n");
                }
                
                if (previous_token == "(" && !postfix.empty())
                {
                    throw expression_error("tom parentes\n");
                }
                
                while (!operator_stack.empty() && operator_stack.top() != "(")
                {
                    postfix += operator_stack.top() + ' ';
                    operator_stack.pop();
                }
                
                if (operator_stack.empty())
                {
                    throw expression_error("hgerparentes saknar matchande vnsterparentes\n");
                }
                // Det finns en vnsterparentes pय़ stacken
                operator_stack.pop();
                --paren_count;
            }
            else if (is_operand(token))
            {
                if (last_was_operand || previous_token == ")")
                {
                    throw expression_error("operand dr operator frvntades\n");
                }
                
                postfix += token + ' ';
                last_was_operand = true;
            }
            else
            {
                throw expression_error("otillय़ten symbol\n");
            }
            
            previous_token = token;
        }
        
        if (postfix == "")
        {
            throw expression_error("tomt infixuttryck!\n");
        }
        
        if (!last_was_operand && !postfix.empty())
        {
            throw expression_error("operator avslutar\n");
        }
        
        if (paren_count > 0)
        {
            throw expression_error("hgerparentes saknas\n");
        }
        
        while (!operator_stack.empty())
        {
            postfix += operator_stack.top() + ' ';
            operator_stack.pop();
        }
        
        if (!postfix.empty())
        {
            postfix.erase(postfix.size() - 1);
        }
        
        return postfix;
    }
    
    // make_expression_tree tar en postfixstrॊng och returnerar ett motsvarande
    // lॊnkat trॊd av Expression_Tree-noder.
    Expression_Tree* make_expression_tree(const std::string& postfix)
    {
        using std::stack;
        using std::string;
        using std::istringstream;
        
        stack<Expression_Tree*> tree_stack;
        string                  token;
        istringstream           ps{postfix};
        try{
            while (ps >> token)
            {
                if (is_operator(token))
                {
                    if (tree_stack.empty())
                    {
                        throw expression_error( "felaktig postfix\n");
                    }
                    Expression_Tree* lhs = nullptr;
                    Expression_Tree* rhs =nullptr;
                    try{
                        rhs = tree_stack.top();
                        
                        tree_stack.pop();
                        
                        if (tree_stack.empty())
                        {
                            throw expression_error( "felaktig postfix\n");
                        }
                        
                        lhs = tree_stack.top();
                    
                    tree_stack.pop();
                    
                    if (token == "^")
                    {
                        tree_stack.push(new Power{lhs, rhs});
                    }
                    else if (token == "*")
                    {
                        tree_stack.push(new Times{lhs, rhs});
                    }
                    else if (token == "/")
                    {
                        tree_stack.push(new Divide{lhs, rhs});
                    }
                    else if (token == "+")
                    {
                        tree_stack.push(new Plus{lhs, rhs});
                    }
                    else if (token == "-")
                    {
                        tree_stack.push(new Minus{lhs, rhs});
                    }
                    else if (token == "=")
                    { 	
                        tree_stack.push(new Assign{lhs, rhs});
                    }
                    }
                    catch(...){
                        delete lhs;
                        delete rhs;
                        throw;}//Moved here, to cover above cases of possible memoryleaks.
                    }
                else if (is_integer(token))
                {
                    tree_stack.push(new Integer{std::stoll(token.c_str())});
                }
                else if (is_real(token))
                {
                    tree_stack.push(new Real{std::stold(token.c_str())});
                }
                else if (is_identifier(token))
                {
                    tree_stack.push(new Variable{token});
                }
                else
                {
                    throw expression_error( "felaktig postfix\n");
                }
            }
            // Det ska bara finnas ett trॊd pो stacken om korrekt postfix.
            
            if (tree_stack.empty())
            {
                throw expression_error( "ingen postfix given\n");
            }
            
            if (tree_stack.size() > 1)
            {
                while (!tree_stack.empty())
                {
                    delete tree_stack.top();
                    tree_stack.pop();
                }
                throw expression_error( "felaktig postfix\n");
            }
            
        }
        catch(...){
            while(not(tree_stack.empty())){
                tree_stack.pop();
            }
            throw;
        }
        
        
        // Returnera trॊdet.
        return tree_stack.top();
        
    }    
} //  end of namespace

Expression make_expression(const string& infix)
{
    try{
        return Expression{make_expression_tree(make_postfix(infix))};
    }
    catch(...){
        throw;}
}
