#include <stack>
#include <list>
#include <map>
#include <cassert>
#include <string.h>
#include <iostream>

#include "include/ssel_commons.h"

using namespace std;

list<map<string, Expression*> >  symtbl_list;

string de_quote(const string &str);
int eval_block(LessBlock &block);
int eval_expr(Expression &expr);
Expression look_up_symtbl(const string &name);
int purify_string(string &name);
void init_priority();

static char priority[OP_DIV + 1];


//delete the quotes surrounding a string
//return a copy of the dequoted string
string de_quote(const string &str)
{
        string result_str = str;
        if(str[0] == '\"' || str[0] == '\'') {
                result_str.erase(0, 1);
                result_str.erase(result_str.size() - 1, 1);
        }
        return result_str;
}

//substitue the @{...} within the string, recursively
int purify_string(string &str)
{
        string::iterator iter_begin, iter_end;
        Expression result_expr;

        for(iter_begin = str.begin(); iter_begin != str.end(); iter_begin++) {

                if(*iter_begin == '@' && *(iter_begin+1) == '{'){
                        iter_end = iter_begin;
                        while(*iter_end != '}') {
                                iter_end++;
                        }

                        string name(iter_begin+2, iter_end);
                        result_expr = look_up_symtbl(name);

                        eval_expr(result_expr);
                        string result_str = *((string *)result_expr[0].data);
                        result_str = de_quote(result_str);

                        str.replace(iter_begin, iter_end+1, result_str.begin(), result_str.end());
                }
        }
        return 0;
}

//look up an Expression named "name", return a copy of the expression
//it's the caller's duty to dequote the name
Expression look_up_symtbl(const string &name)
{
        auto iter=symtbl_list.end();
        map<string, Expression* >::iterator result_iter;

        do{
                --iter;
                result_iter = (*iter).find(name);

                if(result_iter != (*iter).end()) {
                        return *(result_iter->second);
                }

        }while(iter != symtbl_list.begin());

        assert(0);

}

//init the priority map of operators
void init_priority()
{
        priority[LEFT_BRACE] = 0;
        priority[OP_ADD] = priority[OP_SUB] = 1;
        priority[OP_MUL] = priority[OP_DIV] = 2;
        priority[OP_AT] = priority[OP_NEG] = 3;
}

//test status
//expr_eval
////constant +-*/ - ====>passed
////color +-*/ ====> passed
////constant && color +-*/ ====>passed
////mix test ====>passed
////@ operator ====>passed
////name lookup ====>passed
////string insertion(aka. purify_string) ====>passed

int main()
{

        Expression expr;
        ExprElement ele;
        LessConstant *pconstant;
        LessColor *color;
        LessDef *def;
        LessBlock block;
        LessElement element;
        LessCssRule *rule;
        string* str;


//        color = (LessColor *)expr[0].data;
//        cout << pconstant->val << pconstant->unit <<endl;
//        cout << color->r<<" " << color->g<<" " <<color->b << endl;

        return 0;
}

//evaluate an expression, replace it directly  with the evaluated one
int eval_expr(Expression &expr)
{
        stack<struct ExprElement> eval_stack, op_stack;
        Expression NPR_expr;
//convert
        for(auto ele_iter = expr.begin(); ele_iter != expr.end(); ++ele_iter) {
                switch(ele_iter->type) {
                case OP_ADD :
                case OP_SUB :
                case OP_MUL:
                case OP_DIV :
                case OP_AT :
                case OP_NEG :
                        if((op_stack.empty()
                            || priority[ele_iter->type] >= priority[op_stack.top().type])) {
                                op_stack.push(*ele_iter);
                        } else {
                                while(!op_stack.empty()
                                      && priority[ele_iter->type] < priority[op_stack.top().type]) {
                                        NPR_expr.push_back(op_stack.top());
                                        op_stack.pop();
                                }
                        }
                        break;
                case CONSTANT :
                case STRING :
                case COLOR :
                        NPR_expr.push_back(*ele_iter);
                        break;
                case LEFT_BRACE :
                        op_stack.push(*ele_iter);
                        break;
                case RIGHT_BRACE :
                        while(op_stack.top().type != LEFT_BRACE) {
                                NPR_expr.push_back(op_stack.top());
                                op_stack.pop();
                        }
                        op_stack.top();
                        break;
                default :
                        break;
                }
        }

        while(!op_stack.empty()) {
                NPR_expr.push_back(op_stack.top());
                op_stack.pop();
        }

//evaluate

        for(auto ele_iter = NPR_expr.begin(); ele_iter != NPR_expr.end(); ++ele_iter) {
                ExprElementType expr_type = CONSTANT; //the final type of this expression
                string unit = "";
                struct ExprElement operand1, operand2;
                string var_name;
                Expression temp_expression;

                switch(ele_iter->type) {

                case CONSTANT:
                case COLOR:
                case STRING:
                        eval_stack.push(*ele_iter);
                        break;

                case OP_ADD:
                        operand2 = eval_stack.top();
                        eval_stack.pop();
                        operand1 = eval_stack.top();
                        eval_stack.pop();

                        eval_stack.push(operand1 + operand2);
                        break;

                case OP_SUB:
                        operand2 = eval_stack.top();
                        eval_stack.pop();
                        operand1 = eval_stack.top();
                        eval_stack.pop();

                        eval_stack.push(operand1 - operand2);
                        break;

                case OP_MUL:
                        operand2 = eval_stack.top();
                        eval_stack.pop();
                        operand1 = eval_stack.top();
                        eval_stack.pop();

                        eval_stack.push(operand1 * operand2);
                        break;

                case OP_DIV:
                        operand2 = eval_stack.top();
                        eval_stack.pop();
                        operand1 = eval_stack.top();
                        eval_stack.pop();

                        eval_stack.push(operand1 / operand2);
                        break;

                case OP_NEG:
                        operand1 = eval_stack.top();
                        eval_stack.pop();

                        eval_stack.push(-operand1);
                        break;

                case OP_AT:
                        operand1 = eval_stack.top();
                        eval_stack.pop();

                        var_name = *((string *)(operand1.data));
                        purify_string(var_name);
                        var_name = de_quote(var_name);
                        temp_expression = look_up_symtbl(var_name);
                        eval_expr(temp_expression);
                        eval_stack.push(temp_expression[0]);
                        break;


                }


        }
        expr.clear();
        if(eval_stack.top().type == STRING) {
                purify_string(*((string *)eval_stack.top().data));
        }
        expr.push_back(eval_stack.top());
        return 0;

}


//evaluate all the property:expression pair with a block, recursively
int eval_block(LessBlock &block)
{
        symtbl_list.push_back(map<string, Expression *>());
        auto curr_symtbl_iter = symtbl_list.end();
        curr_symtbl_iter--;

        for(auto ele_iter = block.begin(); ele_iter != block.end(); ++ele_iter) {
                if(ele_iter->type ==  DEF) {
                        struct LessDef* def = (struct LessDef*)ele_iter->data;
                        (*curr_symtbl_iter)[def->name] = &(def->expression);
                }
        }

        for(auto ele_iter = block.begin(); ele_iter != block.end(); ++ele_iter) {

                struct LessCssRule* rule;
                LessBlock* block;
                struct LessSelector* selector;

                switch(ele_iter->type) {

                case CSS_RULE :
                        rule = (struct LessCssRule*)ele_iter->data;
                        eval_expr(rule->expression);
                        break;

                case NORMAL_SELECTOR :
                case PARAMETRIC_SELECTOR :
                        selector = (struct LessSelector*)ele_iter->data;
                        purify_string(selector->name);
                        eval_block(selector->selector_body);
                        break;
                default :
                        break;

                }

        }

}
