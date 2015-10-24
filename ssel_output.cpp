#include "./include/ssel_commons.h"

#include <iostream>
#include <fstream>
#include <iterator>
using namespace std;
list<string> G_from_chain;
LessBlock G_buffer;
void write_css_file(fstream &output_stream,const LessBlock& toWrite)
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
                            name=parent_name+" "+name;
                        auto p_inherit=new LessSelector;
                        p_inherit>name=name;
                        p_inherit->selector_body=p_less_selector->selector_body;
                        auto sealed=new LessElement;
                        sealed->type=LessElementType::NORMAL_SELECTOR;
                        sealed->data=p_inherit;
                        G_buffer.push_back(*sealed);
                        write_css_file(output_stream,p_less_selector->selector_body);
                    }
                    else {
                        //Actual Writing work.
                        *++o_iter=name;
                        *++o_iter="{\n"
                        //write_normal_selector(output_stream,p_less_selector);
                        G_from_chain.push_back(p_less_selector->name);
                        write_css_file(output_stream,p_less_selector->selector_body);
                        G_from_chain.pop_back();
                        *++o_iter="}\n";
                    }
                }
                break;
            case LessElementType::PARAMETRIC_SELECTOR:
                {
                    auto p_less_selector=(LessSelector *)elem.data;
                    string name=p_less_selector->name;
                    for(auto parent_name = G_from_chain.rbegin();parent_name !=G_from_chain.rend();++parent_name)
                        name=parent_name+" "+name;
                    //Actual Writing work.
                    *++o_iter=name;
                    *++o_iter="{\n"
                    //write_normal_selector(output_stream,p_less_selector);
                    G_from_chain.push_back(p_less_selector->name);
                    write_css_file(output_stream,p_less_selector->selector_body);
                    G_from_chain.pop_back();
                    *++o_iter="}\n";
                }
                break;
            case LessElementType::BLOCK_COMMENT:
                break;
            case LessElementType::MIXIN:
                break;
            case LessElementType::CSS_RULE:
                break;
            default:
                break;
        }
    }
    //Write things in buffer.
    write_css_file(output_stream,G_buffer);
}
void write_normal_selector(fstream out_stream,LessSelector * p_less_selector)
{
    ostream_iterator<string> o_iter(p_less_selector," ");//Writes an space after every string.
}
