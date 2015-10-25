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

void rgbread(string::iterator& iter, int& color,int& flag){

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

Expression *getexpr(string::iterator &iter){

	Expression* t = new Expression;
       	bool flag = 1;  //whether it's a single string;

        while(isspace(*iter))
                iter++;
	if(*iter=='\0')
                return t;

	if(*iter=='@' && *(iter+1) != '{')  //if begins with @{
                flag=0;
	else if(*iter=='+') flag=0;
	else if(*iter=='*') flag=0;
        else if(*iter=='/') flag=0;
	else if(numberjudge(*iter)) flag=0;
	else if(*iter=='(') flag=0;
	else if(*iter=='-') {
		bool flag2=1;

                if(isspace(*iter))
                        iter++;

		if(*iter=='@')flag2=0;
		if(*iter=='(')flag2=0;
		if((*iter>='0')and(*iter<='9'))flag2=0;

                if(!flag)
                        flag = 0;

	}
        else  if(*iter == '#'){
		bool flag2 = 1;
                int k;

                flag2 = 1;
                for(auto iter2 = iter; iter2 != s.end() && iter2 <= iter+3; ++iter2) {
                        if(!judge16(*iter2))
                                flag2 = 0;
                }

		if(flag2)
                        flag = 0;
	}
        else if(*iter == 'r'){
		bool flag2 = 1;

		if(*(iter+1) != 'g')flag2 = 0;
		if(*(iter+2) != 'b')flag2 = 0;
		if(*(iter+3) != '(')flag2 = 0;

                if(flag2)
                        flag = 0;
	}

	if(flag){
		ExprElement* STR = new ExprElement;
                char end_char;
                auto iter2 = iter;

                if((*iter) == '\'') {
                        end_char = '\'';
                        for(iter2 = iter; *iter2 != end_char; ++iter2);
                } else if ((*iter) == '"') {
                        end_char = '\"';
                        for(iter2 = iter; *iter2 != end_char; ++iter2);
                } else {
                        end_char = ';';
                        for(iter2 = iter; *iter2 != end_char; ++iter2);
                        iter2--; //we don't want;
                }
                //to do, pair the {

                STR->type = STRING;
                STR->data = new string(iter, iter2+1);
                t->push_back(*STR);

                while(*iter2 != ';')
                        iter2++;
                iter = iter2 + 1;
	}
	else{
		while(iter != s.end()){
                        if(isspace(*iter)){
                                iter++;
                                continue;
                        }

                        if(*iter == '@'){
                                ExprElement* expr = new ExprElement;
                                expr->type = OP_AT;
                                t->push_back(*expr);
                                iter++;
                        }
                        else if(*iter == '-'){
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
                                iter++;
                        }
                        else if(*iter == '('){
                                ExprElement* expr = new ExprElement;
                                expr->type = LEFT_BRACE;
                                t->push_back(*expr);
                                iter++;
                        }
                        else if(*iter == ')'){
                                ExprElement* expr = new ExprElement;
                                expr->type = RIGHT_BRACE;
                                t->push_back(*expr);
                                iter++;
                        }
                        else if(*iter == '+'){
                                ExprElement* expr = new ExprElement;
                                expr->type = OP_ADD;
                                t->push_back(*expr);
                                iter++;
                        }
                        else if(*iter == '*'){
                                ExprElement* expr = new ExprElement;
                                expr->type = OP_MUL;
                                t->push_back(*expr);
                                iter++;
                        }
                        else if(*iter == '/'){
                                ExprElement* expr = new ExprElement;
                                expr->type = OP_DIV;
                                t->push_back(*expr);
                                iter++;
                        }
                        else if(numberjudge(*iter)){
                                ExprElement* expr = new ExprElement;

                                if((*t)[t->size() - 1].type  ==  OP_AT) {
					expr->type = STRING;
                                        string*temps = new string;

					while(varjudge(*iter)) {
                                                *temps += *iter;
                                                iter++;
                                        }
					expr->data = reinterpret_cast<void*>(temps);
					t->push_back(*expr);

                                } else {
					expr->type = CONSTANT;
					LessConstant*con = new LessConstant;
					con->val = 0;
					while(numberjudge(*iter)) {
                                                con->val = con->val*10+*iter-'0';
                                                iter++;
                                        }

					double jie = 0.1;
					if(*iter == '.'){
                                                iter++;
						while(numberjudge(*iter)){
							con->val += jie*(*iter-'0');
							jie *= 0.1;
                                                        iter++;
						}
					}

					con->unit = "";
                                        while(letterjudge(*iter)){
                                                con->unit += *iter;
                                                iter++;
                                        }

					expr->data = reinterpret_cast<void*>(con);
					t->push_back(*expr);
                                }
                        }
                        else if(*iter == '#'){
                                ExprElement* expr = new ExprElement;
                                expr->type = COLOR;
                                LessColor*color = new LessColor;
                                color->r = color->g = color->b = 0;

                                int flag2 = 0;
                                if(iter+6 < s.end())
                                        if(judge16(*(iter+3))&&judge16(*(iter+4))&&judge16(*(iter+5)))
                                                flag2 = 1;

                                for(int i = 0;i <= flag2;i++)rgbread(iter,color->r,i);
                                for(int i = 0;i <= flag2;i++)rgbread(iter,color->g,i);
                                for(int i = 0;i <= flag2;i++)rgbread(iter,color->b,i);

                                expr->data = reinterpret_cast<void*>(color);
                                t->push_back(*expr);
                                iter++;
                        }
                        else if(*iter == 'r'){
                                ExprElement* expr = new ExprElement;
                                expr->type = COLOR;
                                LessColor* color = new LessColor;
                                color->r = getnumber(iter);
                                iter++;
                                color->g = getnumber(iter);
                                iter++;
                                color->b = getnumber(iter);
                                iter++;
                                expr->data = reinterpret_cast<void*>(color);
                                t->push_back(*expr);
                        }
                        else if(*iter == '}' || *iter == ';') {
                                break;
                        }
                        else {
                                ExprElement* expr = new ExprElement;
                                expr->type = STRING;
                                string*temps = new string;
                                *temps = "";
                                while(varjudge(*iter)) {
                                        *temps += *iter;
                                        iter++;
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

void read(LessBlock* p,string::iterator iter1, string::iterator2){

        for(auto iter = s->begin(); iter != s->end(); ++iter) {
               if(isspace(*iter))
                       continue;
               else if(* iter == '/') {
                       LessElement* t = new LessElement;
                       if((*(iter+1)== '/')) {
                               while(*iter != '\n')
                                       iter++;
                               continue;
                       }
		       else if(*(iter+1) == '*'){
                               auto iter2 = iter;
                               for(; iter2 != s->end(); ++iter2) {
                                       if(*iter2 == '*' && *(iter2+1) == '/') {
                                               break;
                                       }
                               }
                               t->type = BLOCK_COMMENT;
                               t->data = reinterpret_cast<void* >(new string(iter, iter2+3));
                               p->push_back(*t);
                               iter = iter2+3;
		       }
	       }
               else {
                       for(auto iter2 = iter; iter2 != s->end(); ++iter2) {
                               if(*iter2 == ':') {
                                       auto iter3 = iter2;
                                       iter3--;
                                       while(isspace(*iter3))
                                               iter3--;

                                       if(*iter == '@' && *(iter+1) != '{') {
                                               LessElement element;
                                               element.type = DEF;
                                               LessDef* pdef = new LessDef;

                                               pdef->name = string(iter, iter3+1);

                                               iter = iter2+1;

                                               pdef->expression = *getexpr(iter);
                                               element.data = (void*)pdef;
                                               p->push_back(element);

                                       } else {
                                               LessElement element;
                                               element.type = CSS_RULE;
                                               LessCssRule* prule = new LessCssRule;

                                               prule->name = string(iter, iter3+1);

                                               iter = iter2+1;

                                               prule->expression = *getexpr(iter);
                                               element.data = (void *)prule;
                                               p->push_back(element);

                                       }

                               } else if(*iter2 == '(') {
                                       auto iter3 = iter2;
                                       iter3--;
                                       while(isspace(*iter3))
                                               iter3--;
                                       
                                       LessElement element;
                                       element.type = PARAMETRIC_SELECTOR;
                                       LessSelector* pselector = new LessSelector;

                                       pselector->name = string(iter, iter3 + 1);

                                       iter = iter2+1;

                                       //todo build the params

                               } else if(*iter2 == '{') {
                                       auto iter3 = iter2;
                                       iter3--;
                                       while(isspace(*iter3))
                                               iter3--;


                                       if(*(iter2-1) == '@')
                                               continue;
                                       else {

                                       }
                               }

                       }
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
