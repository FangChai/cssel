#include "include/ssel_commons.h"
#include<cstdio>
#include<iostream>
#include<cstdlib>
#include<cstring>
#include<cctype>
#include <utility>

using namespace std;

string s;

bool letterjudge(const char&c){
	if((c>='a')&&(c<='z'))return 1;
	if((c>='A')&&(c<='Z'))return 1;
	return 0;
}
bool varjudge(const char&c){
	if((c>='A')and(c<='Z'))return 1;
	if((c>='a')and(c<='z'))return 1;
	if((c>='0')and(c<='9'))return 1;
	if((c=='-')or(c=='_'))return 1;
	return 0;
}
bool judge16(const char&c){
	if((c>='0')and(c<='9'))return 1;
	if((c>='a')and(c<='f'))return 1;
	if((c>='A')and(c<='F'))return 1;
	return 0;
}
bool numberjudge(const char&c){
	if((c>='0')and(c<='9'))return 1;
	return 0;
}

void rgbread(string::iterator iter, int& color,int& flag){

        int temp;
	if((*iter>='0')&&(*iter<='9'))temp=*iter-'0';
	if((*iter>='a')&&(*iter<='f'))temp=*iter-'a'+10;
	if((*iter>='A')&&(*iter<='F'))temp=*iter-'A'+10;
	if(flag)
                color=color*16+temp;
	else
                color=temp;
}
int getnumber(string::iterator& iter){
	while(!numberjudge(*iter))
                iter++;
	int temp=0;
	while(numberjudge(*iter)){
		temp=temp*10 + *iter-'0';
		iter++;
	}
	return temp;
}

Expression *getexpr(string::iterator iter_begin, string::iterator iter_end){

	Expression* t = new Expression;
       	bool flag = 1;  //whether it's a single string;

        while(isspace(*iter_begin))
                iter_begin++;

        //empty expression
        if(*iter_begin == ';')
                return t;

	if(*iter_begin=='@' && *(iter_begin+1) != '{')  //if begins with @{
                flag=0;
	else if(*iter_begin=='+') flag=0;
	else if(*iter_begin=='*') flag=0;
        else if(*iter_begin=='/') flag=0;
	else if(numberjudge(*iter_begin)) flag=0;
	else if(*iter_begin=='(') flag=0;
	else if(*iter_begin=='-') {
		bool flag2=1;

                if(isspace(*iter_begin))
                        iter_begin++;

		if(*iter_begin=='@')flag2=0;
		if(*iter_begin=='(')flag2=0;
		if((*iter_begin>='0')and(*iter_begin<='9'))flag2=0;

                if(!flag)
                        flag = 0;

	}
        else  if(*iter_begin == '#'){
		bool flag2 = 1;
                int k;

                flag2 = 1;
                for(auto iter_begin2 = iter_begin; iter_begin2 != iter_end && iter_begin2 <= iter_begin+3; ++iter_begin2) {
                        if(!judge16(*iter_begin2))
                                flag2 = 0;
                }

		if(flag2)
                        flag = 0;
	}
        else if(*iter_begin == 'r'){
		bool flag2 = 1;

		if(*(iter_begin+1) != 'g')flag2 = 0;
		if(*(iter_begin+2) != 'b')flag2 = 0;
		if(*(iter_begin+3) != '(')flag2 = 0;

                if(flag2)
                        flag = 0;
	}

	if(flag){
		ExprElement* STR = new ExprElement;
                char end_char;
                auto iter_begin2 = iter_begin;

                if((*iter_begin) == '\'') {
                        end_char = '\'';
                        for(iter_begin2 = iter_begin; *iter_begin2 != end_char; ++iter_begin2);
                } else if ((*iter_begin) == '"') {
                        end_char = '\"';
                        for(iter_begin2 = iter_begin; *iter_begin2 != end_char; ++iter_begin2);
                } else {
                        end_char = ';';
                        for(iter_begin2 = iter_begin; *iter_begin2 != end_char; ++iter_begin2);
                        iter_begin2--; //we don't want;
                }
                //to do, pair the {

                STR->type = STRING;
                STR->data = new string(iter_begin, iter_begin2+1);
                t->push_back(*STR);

                while(*iter_begin2 != ';')
                        iter_begin2++;
                iter_begin = iter_begin2 + 1;
	}
	else{
		while(iter_begin != iter_end){
                        if(isspace(*iter_begin)){
                                iter_begin++;
                                continue;
                        }

                        if(*iter_begin == '@'){
                                ExprElement* expr = new ExprElement;
                                expr->type = OP_AT;
                                t->push_back(*expr);
                                iter_begin++;
                        }
                        else if(*iter_begin == '-'){
                                ExprElement* expr = new ExprElement;
                                bool nflag = 0;

                                switch((*t)[t->size()-1].type){
                                case OP_ADD:
                                case OP_SUB:
                                case OP_MUL:
                                case OP_DIV:
                                case LEFT_BRACE:
                                        nflag = 1;
                                }

                                if(nflag)
                                        expr->type = OP_NEG;
                                else
                                        expr->type = OP_SUB;
                                t->push_back(*expr);
                                iter_begin++;
                        }
                        else if(*iter_begin == '('){
                                ExprElement* expr = new ExprElement;
                                expr->type = LEFT_BRACE;
                                t->push_back(*expr);
                                iter_begin++;
                        }
                        else if(*iter_begin == ')'){
                                ExprElement* expr = new ExprElement;
                                expr->type = RIGHT_BRACE;
                                t->push_back(*expr);
                                iter_begin++;
                        }
                        else if(*iter_begin == '+'){
                                ExprElement* expr = new ExprElement;
                                expr->type = OP_ADD;
                                t->push_back(*expr);
                                iter_begin++;
                        }
                        else if(*iter_begin == '*'){
                                ExprElement* expr = new ExprElement;
                                expr->type = OP_MUL;
                                t->push_back(*expr);
                                iter_begin++;
                        }
                        else if(*iter_begin == '/'){
                                ExprElement* expr = new ExprElement;
                                expr->type = OP_DIV;
                                t->push_back(*expr);
                                iter_begin++;
                        }
                        else if(numberjudge(*iter_begin)){
                                ExprElement* expr = new ExprElement;

                                if(t->size() - 1 == 0 || (*t)[t->size() - 1].type  ==  OP_AT) {
					expr->type = STRING;
                                        string*temps = new string;

					while(varjudge(*iter_begin)) {
                                                *temps += *iter_begin;
                                                iter_begin++;
                                        }
					expr->data = reinterpret_cast<void*>(temps);
					t->push_back(*expr);

                                } else {
					expr->type = CONSTANT;
					LessConstant*con = new LessConstant;
					con->val = 0;
					while(numberjudge(*iter_begin)) {
                                                con->val = con->val*10+*iter_begin-'0';
                                                iter_begin++;
                                        }

					double jie = 0.1;
					if(*iter_begin == '.'){
                                                iter_begin++;
						while(numberjudge(*iter_begin)){
							con->val += jie*(*iter_begin-'0');
							jie *= 0.1;
                                                        iter_begin++;
						}
					}

					con->unit = "";
                                        while(letterjudge(*iter_begin)){
                                                con->unit += *iter_begin;
                                                iter_begin++;
                                        }

					expr->data = reinterpret_cast<void*>(con);
					t->push_back(*expr);
                                }
                        }
                        else if(*iter_begin == '#'){
                                ExprElement* expr = new ExprElement;
                                expr->type = COLOR;
                                LessColor*color = new LessColor;
                                color->r = color->g = color->b = 0;

                                int flag2 = 0;
                                if(iter_begin+6 < iter_end)
                                        if(judge16(*(iter_begin+3))&&judge16(*(iter_begin+4))&&judge16(*(iter_begin+5)))
                                                flag2 = 1;

                                for(int i = 0;i <= flag2;i++)rgbread(iter_begin+i,color->r,i);
                                iter_begin+=2;
                                for(int i = 0;i <= flag2;i++)rgbread(iter_begin+i,color->g,i);
                                iter_begin+=2;
                                for(int i = 0;i <= flag2;i++)rgbread(iter_begin+i,color->b,i);
                                iter_begin+=2;

                                expr->data = reinterpret_cast<void*>(color);
                                t->push_back(*expr);
                                iter_begin++;
                        }
                        else if(*iter_begin == 'r'){
                                ExprElement* expr = new ExprElement;
                                expr->type = COLOR;
                                LessColor* color = new LessColor;
                                color->r = getnumber(iter_begin);
                                iter_begin++;
                                color->g = getnumber(iter_begin);
                                iter_begin++;
                                color->b = getnumber(iter_begin);
                                iter_begin++;
                                expr->data = reinterpret_cast<void*>(color);
                                t->push_back(*expr);
                        }
                        else if(*iter_begin == '}' || *iter_begin == ';') {
                                break;
                        }
                        else {
                                ExprElement* expr = new ExprElement;
                                expr->type = STRING;
                                string*temps = new string;
                                *temps = "";
                                while(varjudge(*iter_begin)) {
                                        *temps += *iter_begin;
                                        iter_begin++;
                                }
                                expr->data = reinterpret_cast<void*>(temps);
                                t->push_back(*expr);

                        }

                }
	}
	return t;
}

string::iterator find_paired_bracket(string::iterator iter)
{
        int n = 1;
        auto iter2 = iter;
        while(n != 0){
                iter2++;
                if(*iter2 == '\"') {
                        iter2++;
                        while(*iter2 !='\"')
                                iter2++;
                }
                else if(*iter2 == '\'') {
                        iter2++;
                        while(*iter2 !='\'')
                                iter2++;
                }
                else if(*iter2 == '/' && *(iter2 + 1) == '/') {
                        iter2++;
                        while(*iter2 !='\n')
                                iter2++;
                }
                else if(*iter2 == '/' && *(iter2 + 1) == '*') {
                        iter2++;
                        while(*iter2 !='*' || *(iter2 + 1) != '/')
                                iter2++;
                        iter2++;
                }
                else if(*iter2 == '{') {
                        n++;
                }
                else if((*iter2 == '}')) {
                        n--;
                }


        }
        return iter2;
}
string::iterator find_paired_brace(string::iterator iter)
{
        int n = 1;
        auto iter2 = iter;
        while(n != 0){
                iter2++;
                if(*iter2 == '\"') {
                        iter2++;
                        while(*iter2 !='\"')
                                iter2++;
                }
                else if(*iter2 == '\'') {
                        iter2++;
                        while(*iter2 !='\'')
                                iter2++;
                }
                else if(*iter2 == '/' && *(iter2 + 1) == '/') {
                        iter2++;
                        while(*iter2 !='\n')
                                iter2++;
                }
                else if(*iter2 == '/' && *(iter2 + 1) == '*') {
                        iter2++;
                        while(*iter2 !='*' || *(iter2 + 1) != '/')
                                iter2++;
                        iter2++;
                }
                else if(*iter2 == '(') {
                        n++;
                }
                else if((*iter2 == ')')) {
                        n--;
                }


        }
        return iter2;
}

void read(LessBlock* p,string::iterator iter_begin, string::iterator iter_end){

        auto iter = iter_begin;

        while(iter != iter_end) {
                if(iter == iter_end)
                        break;

                if(isspace(*iter) || *iter == ';'){
                        iter++;
                        continue;
                }

                else if(* iter == '/') {
                        LessElement* t = new LessElement;
                        if((*(iter+1)== '/')) {
                                while(*iter != '\n')
                                        iter++;
                                continue;
                                iter++;
                        }
                        else if(*(iter+1) == '*'){
                                auto iter2 = iter;

                                while(*iter2 !=  '*' || *(iter2+1) != '/') {
                                        iter2++;
                                }

                                t->type = BLOCK_COMMENT;
                                t->data = reinterpret_cast<void* >(new string(iter, iter2+2));
                                p->push_back(*t);
                                iter = iter2+1;
                                iter++;
                        }
                }
                else {
                        auto iter2 = iter;
                        while(iter2 != iter_end) {
                                iter2++;
                                if(iter2 == iter_end)
                                        break;

                                if(*iter2 == ':') {
                                        auto iter3 = iter2;
                                        iter3--;
                                        while(isspace(*iter3))
                                                iter3--;

                                        if(*iter == '@' && *(iter+1) != '{') { //def
                                                LessElement element;
                                                element.type = DEF;

                                                LessDef* pdef = new LessDef;
                                                pdef->name = string(iter+1, iter3+1);

                                                //find the region of the def
                                                auto iter_expr_end = iter2+1;
                                                while(iter_expr_end != iter_end && *iter_expr_end != ';' && *iter_expr_end != '}')
                                                        iter_expr_end++;

                                                pdef->expression = *getexpr(iter2+1, iter_expr_end);

                                                element.data = (void*)pdef;
                                                p->push_back(element);

                                                iter = iter_expr_end;
                                                break;

                                        } else { //rule
                                                LessElement element;
                                                element.type = CSS_RULE;
                                                LessCssRule* prule = new LessCssRule;

                                                prule->name = string(iter, iter3+1);

                                                auto iter_expr_end = iter2;
                                                while(iter_expr_end != iter_end && *iter_expr_end != ';' && *iter_expr_end != '}')
                                                        iter_expr_end++;

                                                prule->expression = *getexpr(iter2+1, iter_expr_end);
                                                element.data = (void *)prule;
                                                p->push_back(element);

                                                iter = iter_expr_end;
                                                break;

                                        }

                                } else if(*iter2 == '(') {
                                        LessElement element;

                                        auto iter3 = iter2;
                                        iter3--;
                                        while(isspace(*iter3))
                                                iter3--;

                                        //determin the region of param list
                                        auto iter_paird_brace = find_paired_brace(iter2);

                                        //determin if it's a mixin or a parametric selector
                                        auto iter_left_bracket = iter_paird_brace+1;
                                        for(;
                                            iter_left_bracket != iter_end && isspace(*iter_left_bracket);
                                            ++iter_left_bracket);

                                        if(*iter_left_bracket == '{') {
                                                element.type = PARAMETRIC_SELECTOR;
                                        } else {
                                                element.type = MIXIN;
                                        }

                                        if( element.type == PARAMETRIC_SELECTOR) {
                                                //contruct name
                                                LessSelector* pselector = new LessSelector;
                                                pselector->name = string(iter, iter3 + 1);

                                                LessParam param;
                                                char separator = 0;

                                                //determin the separator of param list
                                                for(auto param_iter = iter2+1; param_iter != iter_paird_brace ; ++param_iter) {
                                                        if(*param_iter == ';')
                                                                separator = ';';
                                                        else if(*param_iter == ',')
                                                                separator = ',';
                                                }

                                                //contruct the paramlist
                                                auto param_iter_begin = iter2;
                                                while(param_iter_begin != iter_paird_brace) {
                                                        param_iter_begin++;
                                                        if(param_iter_begin == iter_paird_brace)
                                                                break;

                                                        if(param_iter_begin == iter_paird_brace)
                                                                break;
                                                        auto param_iter_end = param_iter_begin;
                                                        while(param_iter_end != iter_paird_brace && *param_iter_end != separator)
                                                                param_iter_end++;

                                                        //find ":"
                                                        auto iter_colon = param_iter_begin;
                                                        for(; iter_colon != param_iter_end && *iter_colon != ':' ; iter_colon++) {
                                                        }

                                                        LessParam param;

                                                        param.name = string(param_iter_begin, iter_colon);

                                                        if(iter_colon != param_iter_end) {
                                                                param.expression= *getexpr(iter_colon+1 , param_iter_end);
                                                        }

                                                        pselector->params.push_back(param);
                                                }
                                                //construct the selector body
                                                auto iter_right_bracket = find_paired_bracket(iter_left_bracket);
                                                read(&pselector->selector_body, iter_left_bracket + 1, iter_right_bracket);

                                                element.data = (void *)pselector;
                                                iter = iter_right_bracket;

                                        } else {
                                                LessMixin* pmixin = new LessMixin;
                                                pmixin->name = string(iter, iter3 + 1);

                                                LessParam param;
                                                char separator = 0;

                                                //determin the separator of param list
                                                for(auto param_iter = iter2+1; param_iter != iter_paird_brace ; ++param_iter) {
                                                        if(*param_iter == ';')
                                                                separator = ';';
                                                        else if(*param_iter == ',')
                                                                separator = ',';
                                                }

                                                //contruct the paramlist
                                                auto param_iter_begin = iter2;
                                                while(param_iter_begin != iter_paird_brace) {
                                                        param_iter_begin++;
                                                        if(param_iter_begin == iter_paird_brace)
                                                                break;

                                                        auto param_iter_end = param_iter_begin;
                                                        while(param_iter_end != iter_paird_brace && *param_iter_end != separator)
                                                                param_iter_end++;


                                                        LessParam param;
                                                        param.expression= *getexpr(param_iter_begin , param_iter_end);

                                                        pmixin->params.push_back(param.expression);

                                                }

                                                element.data = (void *)pmixin;
                                                iter = iter_left_bracket; //if it's a mixin ,the iter_left_bracket -> ';'
                                        }
                                        p->push_back(element);
                                        break;

                                } else if(*iter2 == '{') {
                                        auto iter3 = iter2;
                                        iter3--;
                                        while(isspace(*iter3))
                                                iter3--;


                                        if(*(iter2-1) == '@') { //string insertion
                                                iter = find_paired_bracket(iter2);
                                                continue;  //continue to scaner, iter remain unchaged
                                        }
                                        else { //normal selector
                                                LessElement element;
                                                element.type = NORMAL_SELECTOR;

                                                //contruct name
                                                LessSelector* pselector = new LessSelector;
                                                pselector->name = string(iter, iter3 + 1);


                                                //construct the selector body
                                                auto iter_right_bracket = find_paired_bracket(iter2);

                                                read(&pselector->selector_body, iter2+1, iter_right_bracket);
                                                element.data = (void *)pselector;
                                                p->push_back(element);

                                                element.data = (void *)pselector;
                                                iter = iter_right_bracket;
                                                break;

                                        }
                                }

                        }

                        iter++;
                }

        }
}


extern void print_block(LessBlock &block);
int main(){
        LessBlock* p = new LessBlock;
        char c;
        while(EOF !=  (c = getc(stdin)))s += c;
        read(p, s.begin(), s.end());
        print_block(*p);
        return 0;
}
