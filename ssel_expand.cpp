#include "./include/ssel_commons.h"

#include <map>
#include <utility>
#include <memory>


//In order to allow same names in seletor and functional mixin.
typedef map<pair<string,LessElementType>,void *> TokenMap;
typedef LessDef *PDef ;
typedef LessSelector* PSelector ;
typedef LessMixin *PMixin ;
void build_def_token(PDef def,TokenMap &tokenmap);
void build_parametricselector_token(PSelector paraselector,TokenMap &tokenmap);
void build_normalselector_token(PSelector  nselector,TokenMap &tokenmap);
void *find_double_call_element(string elem_name,LessElementType elem_type);
void less_expand(LessBlock& currenBlock);

LessBlock expand_mixin(PMixin mixin);
//TODO
//Debug purpose main function
//Remove before release.
int main(int argc,char** argv)
{
    //Construct test data.

    //Test.1. selector expand.(no param)
    auto selector1=new LessSelector;
    auto selector1_elem=new LessElement;
    auto selector2=new LessSelector;
    auto selector2_elem=new LessElement;
    auto mixin_call=new LessMixin;
    auto mixin_call_elem=new LessElement;
    auto css_rule=new LessCssRule;
    auto css_rule_elem=new LessElement;
    auto css_rule_expr_elem=new ExprElement;
    auto css_rule_expr_elem_filler=new string;
    (*css_rule_expr_elem_filler)="background=gray;";
    css_rule_expr_elem->type=ExprElementType::STRING;
    css_rule_expr_elem->data=css_rule_expr_elem_filler;
    css_rule->name="filter";
    css_rule->expression.push_back(*css_rule_expr_elem);
    css_rule_elem->type=LessElementType::CSS_RULE;
    css_rule_elem->data=css_rule;
    mixin_call->name=".selector2";
    mixin_call_elem->type=LessElementType::MIXIN;
    mixin_call_elem->data=mixin_call;
    selector1->name=".selector1";
    selector2->name=".selector2";
    selector1->selector_body.push_back(*mixin_call_elem);
    selector2->selector_body.push_back(*css_rule_elem);
    LessBlock testful_block;
    testful_block.push_back(*selector1_elem);
    testful_block.push_back(*selector2_elem);
    less_expand(testful_block);
    return 0;
}

list<TokenMap> G_tokenList;
void less_expand(LessBlock& currenBlock)
    //To Tang Pro : Specify the second param as true.
{
    TokenMap tokenizer;
    for(auto &elem : currenBlock){
        switch(elem.type){
            case LessElementType::DEF:
                build_def_token((PDef)elem.data,tokenizer);
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
    for(auto elem =currenBlock.begin();elem!=currenBlock.end();++elem)
    {
        switch(elem->type){
            case LessElementType::DEF:
                break;
            case LessElementType::MIXIN:
                {
					auto block=expand_mixin((PMixin)elem->data);
					LessSelector *scoped_block=new LessSelector({"",vector<LessParam>(),block});
					LessElement sealed;
					sealed.type = LessElementType::NORMAL_SELECTOR;
					sealed.data = scoped_block;
					elem=currenBlock.erase(elem);
					currenBlock.insert(elem,sealed );
                }
                break;
            case LessElementType::BLOCK_COMMENT:
                break;
            case LessElementType::CSS_RULE:
                break;
            case LessElementType::NORMAL_SELECTOR:
                less_expand(((PSelector)elem->data)->selector_body);
                break;
            case LessElementType::PARAMETRIC_SELECTOR:
                less_expand(((PSelector)elem->data)->selector_body);
                break;
        }
    }
    //Throw away token list never to be use.
    //Make sure the token list is always at the same depth as our analyze goes.
    G_tokenList.pop_back();
}
LessBlock expand_mixin(PMixin mixin)
{
    //TODO
    //Use a parallel set list to determine whether a name is in the token list.
    //Will improve the performance .
    if(mixin->params.empty()){
        auto ptr_para_selector =(PSelector)find_double_call_element(mixin->name,LessElementType::PARAMETRIC_SELECTOR);
                    LessBlock merged_body;
                    if(ptr_para_selector){
                        auto body=ptr_para_selector->selector_body;
                        auto params=ptr_para_selector->params;
                        for(auto &elem : params){
                            LessDef *to_ins=new LessDef({elem.name,elem.expression});
							LessElement sealed;
							sealed.type = LessElementType::DEF;
							sealed.data = to_ins;
                            body.insert(body.begin(),sealed);
                        }
                        /*
                        //Handling recursive calls.
                        for(auto iter3=body.begin();iter3!=body.end();++iter3){
                            if(iter3->type==LessElementType::MIXIN){
                                auto block = expand_mixin((PMixin)iter3->data);
                                iter3=body.erase(iter3);
                                LessSelector scoped_block={"",vector<LessParam>(),block};
                                body.insert(iter3,block);
                            }
                        }
                        */
                        merged_body.insert(merged_body.end(),body.begin(),body.end());
                    }
                    auto ptr_norm_selector=(PSelector)find_double_call_element(mixin->name,LessElementType::NORMAL_SELECTOR);
                    if(ptr_norm_selector){
                        auto body=(ptr_norm_selector)->selector_body;
                        merged_body.insert(merged_body.end(),body.begin(),body.end());
                    }
                    return merged_body;
    }
    else{
        //Handling non-default parametric calls.
        //Check if a valid call first.
        LessBlock merged_body;
        auto ptr_norm_selector=(PSelector)find_double_call_element(mixin->name,LessElementType::NORMAL_SELECTOR);
        auto ptr_para_selector=(PSelector)find_double_call_element(mixin->name,LessElementType::PARAMETRIC_SELECTOR);
        if(ptr_para_selector||ptr_para_selector->params.size()==mixin->params.size()){
            LessBlock body=ptr_para_selector->selector_body;
            for(int i=0;i<mixin->params.size();++i){
                LessDef *converted_Param=new LessDef({ptr_para_selector->params[i].name,mixin->params[i]});
				LessElement sealed;
				sealed.type = LessElementType::DEF;
				sealed.data = converted_Param;
                body.insert(body.begin(),sealed);
            }
            merged_body.insert(merged_body.end(),body.begin(),body.end());
        }
        if(ptr_norm_selector){
            auto body = ptr_norm_selector->selector_body;
            merged_body.insert(merged_body.end(),body.begin(),body.end());
        }
        return merged_body;
    }
}
void *find_double_call_element(string elem_name,LessElementType elem_type)
{
    auto index=make_pair(elem_name,elem_type);
    for(auto iter=G_tokenList.rbegin();iter!=G_tokenList.rend();++iter){
        if(iter->find(index)!=iter->end()){
            return iter->at(index);
        }
    }
    return nullptr;
}
void build_normalselector_token(PSelector  nselector,TokenMap &tokenmap)
{
    tokenmap[make_pair(nselector->name,LessElementType::NORMAL_SELECTOR)]=nselector;
}
void build_parametricselector_token(PSelector paraselector,TokenMap &tokenmap)
{
    tokenmap[make_pair(paraselector->name,LessElementType::PARAMETRIC_SELECTOR)]=paraselector;
}
void build_def_token(PDef def,TokenMap &tokenmap)
{
    tokenmap[make_pair(def->name,LessElementType::DEF)]=def;
}
