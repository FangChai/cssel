#include "./include/ssel_commons.h"

#include <iostream>
#include <fstream>
#include <iterator>
using namespace std;
list<string> G_from_chain;
LessBlock G_buffer;
void write_css_file(ofstream &output_stream,const LessBlock& toWrite,bool firstInvoke)
{
    ostream_iterator<string> o_iter(output_stream," ");//Writes an space after every string.
    for(auto &elem : toWrite){
        switch(elem.type){
            case LessElementType::DEF:
                {
                    //What!?
                    //Are you kidding ?
                    //Can here be a define!?
                }
                break;
            case LessElementType::NORMAL_SELECTOR:
                {
                    auto p_less_selector=(LessSelector *)elem.data;
                    string name=p_less_selector->name;
                    if(!G_from_chain.empty()){
                        for(auto parent_name = G_from_chain.rbegin();parent_name !=G_from_chain.rend();++parent_name)
                            name=*parent_name+" "+name;
                        auto p_inherit=new LessSelector;
                        p_inherit->name=name;
                        p_inherit->selector_body=p_less_selector->selector_body;
                        auto sealed=new LessElement;
                        sealed->type=LessElementType::NORMAL_SELECTOR;
                        sealed->data=p_inherit;
                        G_buffer.push_back(*sealed);
                        write_css_file(output_stream,p_less_selector->selector_body,false);
                    }
                    else {
                        //Actual Writing work.
                        *++o_iter=name;
                        *++o_iter="{\n";
                        //write_normal_selector(output_stream,p_less_selector);
                        G_from_chain.push_back(p_less_selector->name);
                        write_css_file(output_stream,p_less_selector->selector_body,false);
                        G_from_chain.pop_back();
                        *++o_iter="}\n";
                    }
                }
                break;
            case LessElementType::PARAMETRIC_SELECTOR:
                {
                    auto p_less_selector=(LessSelector *)elem.data;
                    write_css_file(output_stream,p_less_selector->selector_body,false);
                }
                break;
            case LessElementType::BLOCK_COMMENT:
                {
                    auto p_comment= (string *)elem.data;
                    *++o_iter=*p_comment;
                }
                break;
            case LessElementType::MIXIN:
                {
                    //There wont be a mixin too.
                }
                break;
            case LessElementType::CSS_RULE:
                {
                    if(G_from_chain.empty()){
                    auto p_css_rule=(LessCssRule *)elem.data;
                    auto css_name = p_css_rule->name+" : ";
                    *++o_iter=css_name;
                    for (auto &rule : p_css_rule->expression){
                        switch(rule.type){
                            case ExprElementType::CONSTANT:
                                {
                                    auto p_constant=(LessConstant *)rule.data;
                                    auto val=p_constant->val;
                                    auto unit=p_constant->unit;
                                    output_stream<<val<<unit<<"; ";//Not using stream iterator here.val<<unit<<endl;//Not using stream iterator here.
                                                                //Might cause trouble.
                                }
                                break;
                            case ExprElementType::STRING:
                                {
                                    auto p_string=(string *) rule.data;
                                    *++o_iter=(*p_string)+";";
                                }
                                break;
                            case ExprElementType::COLOR:
                                {
                                    auto p_color=(LessColor *)rule.data;
                                    output_stream<<"rgb("<<p_color->r<<","<<p_color->g<<","<<p_color->b<<"); ";
                                }
                                break;
                            default:
                                //What the hell?
                                break;
                    }
                    }
                break;
                }
            }
        }
    }
    //Write things in buffer.
    if(firstInvoke)
        write_css_file(output_stream,G_buffer,false);
}
int main() //For Debug
{
    LessBlock test_data;
    auto elem = new LessElement;
    auto css_rule=new LessCssRule;
    css_rule->name="rule1";
    auto  css_rule_expr_elem=new ExprElement;
    css_rule_expr_elem->type=ExprElementType::STRING;
    css_rule_expr_elem->data=new string("testful string");
    css_rule->expression.push_back(*css_rule_expr_elem);
    elem->data=css_rule;
    elem->type=LessElementType::CSS_RULE;
    test_data.push_back(*elem);
    ofstream file("output");
    write_css_file(file,test_data,true);
    return 0;
}
