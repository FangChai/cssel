#include "./include/ssel_commons.h"

#include <map>
list<map<string,void *>> G_tokenList;

typedef map<string,void *> TokenMap;
typedef *PNSelector LessSelector;
void less_expand(LessBlock& currenBlock)
{
    TokenMap tokenizer;
    for(auto &elem : currenBlock){
        switch(elem.type){
            case LessElementType::DEF:
                break;
            case LessElementType::NORMAL_SELECTOR:
                build_normalselector_token((PNSelector)elem.data,tokenizer );
                break;
            case LessElementType::BLOCK_COMMENT:
                break;
            case LessElementType::MIXIN:
                break;
            case LessElementType::CSS_RULE:
                break;
            case LessElementType::PARAMETRIC_SELECTOR:
                break;
            default:
                break;
        }
    }
}
void build_normalselector_token(PNSelector  nselector,TokenMap &tokenmap)
{
    tokenmap[nselector->name]=nselector;
}
void build_parametricselector_token(PNSelector )
