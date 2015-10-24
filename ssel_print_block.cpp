#include "include/ssel_commons.h"
#include <cstdio>
#include <iostream>

using namespace std;

void print_block(LessBlock &block);
void print_def(LessDef &def);
void print_param(LessParam &param);
void print_parametric_selector(LessSelector &selector);
void print_normal_seclector(LessSelector &selector);
void print_block_comment(string &str);
void print_mixin(LessMixin &mixin);
void print_css_rule(LessCssRule &rule);
void print_expression(Expression &expr);

void print_block(LessBlock &block)
{
        cout<<"block begin\n"<<endl;

        for(auto block_iter = block.begin(); block_iter != block.end(); ++block_iter) {
                switch(block_iter->type) {
                case DEF:
                        print_def(*(struct LessDef*)(block_iter->data));
                        break;
                case NORMAL_SELECTOR :
                        print_normal_seclector(*(struct LessSelector *)(block_iter->data));
                        break;
                case PARAMETRIC_SELECTOR:
                        print_parametric_selector(*(struct LessSelector *)(block_iter->data));
                        break;
                case BLOCK_COMMENT:
                        print_block_comment(*(string *)block_iter->data);
                        break;
                case MIXIN:
                        print_mixin(*(struct LessMixin *)block_iter->data);
                        break;
                case CSS_RULE:
                        print_css_rule(*(struct LessCssRule*)block_iter->data);
                        break;
                default:
                        break;
                }
        }
        cout<<"block end\n"<<endl;
}

void print_def(LessDef &def)
{
        cout<<"def begin\n"<<endl;
        cout<<def.name<<endl;
        print_expression(def.expression);
        cout<<"def end\n"<<endl;
}

void print_param(LessParam &param)
{
        cout<<"param begin\n"<<endl;
        cout<<param.name<<endl;
        print_expression(param.expression);
        cout<<"param end\n"<<endl;

}

void print_parametric_selector(LessSelector &selector)
{
        cout<<"para selector begin\n"<<endl;
        cout<<selector.name<<endl;
        for(auto iter = selector.params.begin(); iter != selector.params.end(); ++iter) {
                print_param(*iter);
        }
        print_block(selector.selector_body);
        cout<<"para selector end\n"<<endl;
}

void print_normal_seclector(LessSelector &selector)
{
        cout<<"normal selector begin\n"<<endl;
        cout<<selector.name<<endl;
        print_block(selector.selector_body);
        cout<<"normal selector end\n"<<endl;
}

void print_block_comment(string &str)
{
        cout<<"comment begin\n"<<endl;
        cout<<str<<endl;
        cout<<"comment end\n"<<endl;
}

void print_mixin(LessMixin &mixin)
{
        cout<<"mixin begin\n"<<endl;
        cout<<mixin.name<<endl;
        for(auto iter = mixin.params.begin() ; iter != mixin.params.end(); ++iter) {
                print_expression(*iter);
        }
        cout<<"mixin end\n"<<endl;
}

void print_css_rule(LessCssRule &rule)
{
        cout<<"rule begin\n"<<endl;
        cout<<rule.name<<endl;
        print_expression(rule.expression);
        cout<<"rule end\n"<<endl;
}

void print_expression(Expression &expr)
{
        LessConstant* pconst;
        LessColor* pcolor;

        cout<<"expression begin\n"<<endl;
        for(auto iter = expr.begin(); iter != expr.end(); ++iter) {
                switch(iter->type) {
                case CONSTANT:
                        pconst = (LessConstant *)iter->data;
                        cout<<"constant:"<<pconst->val<<pconst->unit<<endl;
                        break;
                case STRING:
                        cout<<"string:"<<*((string*)iter->data)<<endl;
                        break;
                case LEFT_BRACE:
                        cout<<"left brace"<<endl;
                        break;
                case RIGHT_BRACE:
                        cout<<"right brace"<<endl;
                        break;
                case OP_AT:
                        cout<<"operator @"<<endl;
                        break;
                case OP_ADD:
                        cout<<"operator +"<<endl;
                        break;
                case OP_NEG:
                        cout<<"operator negative"<<endl;
                        break;
                case OP_SUB:
                        cout<<"operator sub"<<endl;
                        break;
                case OP_MUL:
                        cout<<"operator *"<<endl;
                        break;
                case OP_DIV:
                        cout<<"operator /"<<endl;
                        break;
                default:
                        break;
                }
        }
        cout<<"expression end\n"<<endl;
}
