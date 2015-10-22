#include "./include/ssel_commons.h"

#include <map>
#include <pair>
//In order to allow same names in seletor and functional mixin.
typedef map<pair<string,LessElementType>,void *> TokenMap;
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
    //TODO
    //Use a parallel set list to determine whether a name is in the token list.
    //Will improve the performance .
    if(mixin->params.empty())
        for(auto iter=G_tokenList.rbegin();iter!=G_tokenList.rend();++iter){
            for(auto iter2=(*iter1).rbegin();iter2!=(*iter1).rend();++iter2){
                if(iter2->find(make_pair(mixin->name,LessElementType::NORMAL_SELECTOR))!=map::end||
                   iter2->find(make_pair(mixin->name,LessElementType::PARAMETRIC_SELECTOR))!=map::end){
                    //Handling default parametric selector.
                    if(iter2->at(make_pair(mixin->name,LessElementType::PARAMETRIC_SELECTOR)){
                        auto selector = PSelector(
                        iter2->at(make_pair(mixin->name,LessElementType::PARAMETRIC_SELECTOR)));
                        auto body=seletor->selector_body;
                        auto params=seletor->params;
                        for(auto &elem : params){
                            LessDef to_ins({elem.name,elem.expression});
                            body.insert(body.begin(),to_ins);
                        }
                        //Handling recursive calls.
                        for(auto iter3=body.begin();iter3!=body.end();++iter3){
                            if(iter3->type==LessElementType::MIXIN){
                                auto block = expand_mixin((PMixin)iter3->data);
                                iter3=body.erase(iter3);
                                LessSelector scoped_block={"",vector<LessParam>(),block};
                                body.insert(iter3,block);
                            }
                        }
                      return body;
                    }
                    else{
                        auto body=(LessSelector(iter2->at(make_pair(mixin->name,LessElementType::NORMAL_SELECTOR)))->selector_body;
                        //Handling recursive calls.
                        for(auto iter3=body.begin();iter3!=body.end();++iter3){
                            if(iter3->type==LessElementType::MIXIN){
                                auto block = expand_mixin((PMixin)iter3->data);
                                iter3=body.erase(iter3);
                                LessSelector scoped_block={"",vector<LessParam>(),block};
                                body.insert(iter3,block);
                            }
                        }
                        return body;
                    }
                }
                else {
                //TODO:
                //Add exception handling code
                //When a call is not found in scope.
                }
            }
        }
    else{
        //Handling non-default parametric calls.
        //Check if a valid call first.

    }
}
void build_normalselector_token(PSelector  nselector,TokenMap &tokenmap)
{
    tokenmap[make_pair(nselector->name,LessElementType::NORMAL_SELECTOR)]=nselector;
}
void build_parametricselector_token(PSelector paraselector,TokenMap &tokenmap)
{
    tokenmap[make_pair(paraselector->name,LessElementType::PARAMETRIC_SELECTOR)]=nselector;
}
void build_def_token(PDef def,TokenMap &tokenmap)
{
    tokenmap[make_pair(def->name,LessElementType::DEF]=def;
}
