#include "./include/ssel_commons.h"

#include <map>
#include <pair>
typedef map<string,pair<LessElementType,void *>> TokenMap;
typedef *PDef LessDef;
typedef *PSelector LessSelector;
typedef *PMixin LessMixin;
list<TokenMap> G_tokenList;
void less_expand(LessBlock& currenBlock,bool firstInvoke=false)
    //To Tang Pro : Specify the second param as true.
{
    TokenMap tokenizer;
    for(auto &elem : currenBlock){
        switch(elem.type){
            case LessElementType::DEF:
                build_def_token((PDef)elem.data,tokenize);
                break;
            case LessElementType::NORMAL_SELECTOR:
                build_normalselector_token((PSelector)elem.data,tokenizer );
                break;
            case LessElementType::BLOCK_COMMENT:
                break;
            case LessElementType::MIXIN:
                break;
            case LessElementType::CSS_RULE:
                break;
            case LessElementType::PARAMETRIC_SELECTOR:
                build_parametricselector_token((PSelector)elem.data,tokenizer);
                break;
            default:
                break;
        }
    }
    //Add the tokenmap to token list.
    G_tokenList.push_back(tokenizer);
    //Second time scan.
    for(auto &elem : currenBlock)
    {
        switch(elem.type){
            case LessElementType::DEF:
                break;
            case LessElementType::MIXIN:
                auto block=expand_mixin((PMixin)elem.data);
                break;
            case LessElementType::BLOCK_COMMENT:
                break;
            case LessElementType::CSS_RULE:
                break;
            case LessElementType::NORMAL_SELECTOR:
                break;
            case LessElementType::PARAMETRIC_SELECTOR:
                break;
        }
    }
}
LessBlock expand_mixin(PMixin mixin)
{
    if(mixin->params.empty())
        for(auto iter=G_tokenList.rbegin();iter!=G_tokenList.rend();++iter){
            for(auto iter2=(*iter1).rbegin();iter2!=(*iter1).rend();++iter2){
                if(iter2->find(mixin->name)!=map::end&&(iter2->at(mixin->name).first==LessElementType::PARAMETRIC_SELECTOR
                        ||iter2->at(mixin->name==LessElementType::NORMAL_SELECTOR)){
                        if(iter2->at(mixin->name==LessElementType::NORMAL_SELECTOR){
                            //TODO:
                            //Call skyegg calc before returning
                            return (LessSelector(iter2->at(mixin->name)->second))->selector_body;
                            }
                        else{
                            auto selector = PSelector(iter2->at(mixin->name)->second);
                            for(auto &elem : selector->selector_body){

                            }
                        }
                }
            }
        }
    else{

    }
}
void build_normalselector_token(PSelector  nselector,TokenMap &tokenmap)
{
    tokenmap[nselector->name]=make_pair(LessElementType::NORMAL_SELECTOR,nselector);
}
void build_parametricselector_token(PSelector paraselector,TokenMap &tokenmap)
{
    tokenmap[paraselector->name]=make_pair(LessElementType::PARAMETRIC_SELECTOR, nselector);
}
void build_def_token(PDef def,TokenMap &tokenmap)
{
    tokenmap[def->name]=make_pair(LessElementType::DEF,def);
}
