#include <stack>
#include <list>
#include <map>
#include <cassert>
#include <string.h>

#include "include/ssel_commons.h"

using namespace std;

list<map<string, Expression *> >  symtbl_list;

static char priority[OP_DIV + 1];

int eval_block(LessBlock* block);
int eval_expr(Expression* expr);
Expression* look_up_symtbl(string* name);
int purify_string(string *);
void init_priority();

int purify_string(string *)
{
        return 0;
}
Expression* look_up_symtbl(string* name)
{
        return 0;
}
void init_priority()
{
        priority[LEFT_BRACE] = 0;
        priority[OP_ADD] = priority[OP_SUB] = 1;
        priority[OP_MUL] = priority[OP_DIV] = 2;
        priority[OP_AT] = priority[OP_NEG] = 3;
}

int main()
{
        return 0;
}

int eval_expr(Expression* expr)
{
        stack<struct ExprElement> eval_stack, op_stack;
        Expression NPR_expr;
//convert
        for(auto ele_iter = expr->begin(); ele_iter != expr->end(); ++ele_iter) {
                switch(ele_iter->type) {
                case OP_ADD :
                case OP_SUB :
                case OP_MUL:
                case OP_DIV :
                case OP_AT :
                case OP_NEG :
                        if((priority[ele_iter->type] >= priority[op_stack.top().type])
                           || op_stack.empty()) {
                                op_stack.push(*ele_iter);
                        } else {
                                while(priority[ele_iter->type] < priority[op_stack.top().type] ||
                                      !op_stack.empty()) {
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
                                              //they won't do 1cm + 2mm, would they?
                        string unit = "";
                        struct ExprElement operand1, operand2;

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
                        }
        }
        return 0;
}

int eval_block(LessBlock* block)
{
        symtbl_list.push_back(map<string, Expression *>());
        auto curr_symtbl_iter = symtbl_list.end();
        curr_symtbl_iter--;

        for(auto ele_iter = block->begin(); ele_iter != block->end(); ++ele_iter) {
                if(ele_iter->type ==  DEF) {
                        struct LessDef* def = (struct LessDef*)ele_iter->data;
                        (*curr_symtbl_iter)[def->name] = &(def->expression);
                }
        }

        for(auto ele_iter = block->begin(); ele_iter != block->end(); ++ele_iter) {

                struct LessCssRule* rule;
                LessBlock* block;
                struct LessSelector* selector;

                switch(ele_iter->type) {

                case CSS_RULE :
                        rule = (struct LessCssRule*)ele_iter->data;
                        assert(eval_expr(&(rule->expression)));
                        break;

                case NORMAL_SELECTOR :
                case PARAMETRIC_SELECTOR :
                        selector = (struct LessSelector*)ele_iter->data;
                        purify_string(&selector->name);
                        eval_block(&selector->selector_body);
                        break;
                default :
                        break;

                }

        }

}
