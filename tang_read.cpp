#include "f.h"
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include <utility>
using namespace std;
char popchar(string*s){
	char c=s[0][0];s->erase(0,1);
	return c;
}
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
char filt(char&c,string*s,string*temps){
	c=popchar(s);
	while((c==' ')or(c=='\n')){
		if((c==' ')or(c=='\n'))while(c==' '){*temps+=c;c=popchar(s);}
		if((c==' ')or(c=='\n'))while(c=='\n'){*temps+=c;c=popchar(s);}
	}
	return c;
}
char filt(string*s){
	while((s[0][0]==' ')or(s[0][0]=='\n'))s->erase(0,1);
	if(s->empty())return '\0';
	char c=s[0][0];s->erase(0,1);
	return c;
}
bool numberjudge(const char&c){
	if((c>='0')and(c<='9'))return 1;
	return 0;
}
bool rgbparam(char&scaner,string*s,string*exprstr,int&number){
	*exprstr+=filt(scaner,s,exprstr);
	if(numberjudge(scaner)){number=scaner-'0';}
					else{return 0;}
	while(numberjudge(scaner=popchar(s))){
			number=number*10+scaner-'0';
			*exprstr+=scaner;
		}
	*exprstr+=scaner;
	if((scaner==' ')or(scaner=='\n'))*exprstr+=filt(scaner,s,exprstr);
	return 1;
}
void rgbread(string*s,int&color,int&flag){
	char c=popchar(s);int temp;
	if((c>='0')&&(c<='9'))temp=c-'0';
	if((c>='a')&&(c<='f'))temp=c-'a'+10;
	if((c>-'A')&&(c<='F'))temp=c-'A'+10;
	if(flag){color=color*16+temp;}
		else{color=temp*16+temp;}
}
int getnumber(string*s){
	while(!numberjudge(s[0][0]))s->erase(0,1);
	int temp=0;
	while(numberjudge(s[0][0])){
		temp=temp*10+s[0][0]-'0';
		s->erase(0,1);
	}
	return temp;
}
Expression*getexpr(string*s){
	Expression*t=new Expression;t->clear();
	char scaner;string*exprstr=new string;	
	filt(scaner,s,exprstr);if(scaner=='\0')return t;
	bool flag=1;*exprstr="";
	if(scaner=='@')
		if(s[0][0]!='{') 
			flag=0; 
	if(scaner=='+')flag=0;
	if(scaner=='*')flag=0;
	if(scaner=='/')flag=0;
	if(numberjudge(scaner))flag=0;
	if(scaner=='(')flag=0;
	if(scaner=='-'){
		bool flag2=1;*exprstr+='-';filt(scaner,s,exprstr);
		if(scaner=='@')flag2=0; 
		if(scaner=='(')flag2=0;
		if((scaner>='0')and(scaner<='9'))flag2=0;
		if(!flag2){
		    ExprElement*neg=new ExprElement;
		    neg->type=OP_NEG;neg->data=NULL;
		    t->push_back(*neg);delete neg; 
	     	flag=0;
	    }
	}
	if(scaner=='#'){
		bool flag2=1;int k;
		for(int i=0;(i<=2)&&(s[0][i]!='\n');i++)
			{
				k=i;
				if(!judge16(s[0][i]))flag2=0;
			}
		if(k!=2)flag2=0; 
		if(flag2){flag=0;} 
			else{*exprstr+='#';}
	}
	if(scaner=='r'){
		bool flag2=1;*exprstr+='r';
		ExprElement*RGB=new ExprElement;
		RGB->type=COLOR;
		LessColor*color=new LessColor;
		if(scaner=popchar(s)!='g')flag2=0;*exprstr+=scaner;
		if(scaner=popchar(s)!='b')flag2=0;*exprstr+=scaner;
		*exprstr+=filt(scaner,s,exprstr);
		if(scaner!='(')flag2=0;
		if(flag2){
			flag2=rgbparam(scaner,s,exprstr,color->r);
			if(scaner!=',')flag2=0;
		}
		if(flag2){
			flag2=rgbparam(scaner,s,exprstr,color->g);
			if(scaner!=',')flag2=0;
		}
		if(flag2){
			flag2=rgbparam(scaner,s,exprstr,color->b);
			if(scaner!=')')flag2=0;
		}
		if(flag2){
			RGB->data=reinterpret_cast<void*>(color);
			t->push_back(*RGB);delete RGB;
			flag=0;
		}
	}
	if(flag){
		while(!s->empty()){
		    *exprstr+=scaner=popchar(s);
		}
		ExprElement*STR=new ExprElement;
	    STR->type=STRING;
	    STR->data=reinterpret_cast<void*>(exprstr);
	    t->push_back(*STR); delete STR;
	} 
	else{
		delete exprstr;
		while(!s->empty()){
				if((scaner==' ')or(scaner=='\n'))scaner=filt(s);
				if(scaner=='@'){
					ExprElement*expr=new ExprElement; 
					expr->type=OP_AT;expr->data=NULL;
					t->push_back(*expr);delete expr; 
					expr=new ExprElement;
					expr->type=STRING;string*temps=new string;*temps=""; 
					while(varjudge(scaner=popchar(s)))*temps+=scaner;
					expr->data=reinterpret_cast<void*>(temps);
					t->push_back(*expr);delete expr;
					scaner=popchar(s);
					continue;
				}
				if(scaner=='-'){
					ExprElement*expr=new ExprElement;
					expr->data=NULL;
					bool nflag=0;
					switch((*t)[t->size()-1].type){
						case OP_ADD:nflag=1;
						case OP_SUB:nflag=1;
						case OP_MUL:nflag=1;
						case OP_DIV:nflag=1;
						case LEFT_BRACE:nflag=1;
					}
					if(nflag){expr->type=OP_NEG;}
						else{expr->type=OP_SUB;}
					t->push_back(*expr);delete expr;
					scaner=popchar(s);
					continue;						
				}
				if(scaner=='('){
					ExprElement*expr=new ExprElement;
					expr->type=LEFT_BRACE;expr->data=NULL;
					t->push_back(*expr);delete expr;
					scaner=popchar(s); 	
					continue;
				}
				if(scaner==')'){
					ExprElement*expr=new ExprElement;
					expr->type=RIGHT_BRACE;expr->data=NULL;
					t->push_back(*expr);delete expr;
					scaner=popchar(s);
					continue;
				}
				if(scaner=='+'){
					ExprElement*expr=new ExprElement;
					expr->type=OP_SUB;expr->data=NULL;
					t->push_back(*expr);delete expr;
					scaner=popchar(s); 
					continue;
				} 
				if(scaner=='*'){
					ExprElement*expr=new ExprElement;
					expr->type=OP_MUL;expr->data=NULL;
					t->push_back(*expr);delete expr;
					scaner=popchar(s); 
					continue;
				}
				if(scaner=='/'){
					ExprElement*expr=new ExprElement;
					expr->type=OP_DIV;expr->data=NULL;
					t->push_back(*expr);delete expr;
					scaner=popchar(s); 
					continue;
				}
				if(numberjudge(scaner)){
					ExprElement*expr=new ExprElement;
					expr->type=CONSTANT;
					LessConstant*con=new LessConstant;
					con->val=scaner-'0';
					while(numberjudge(scaner=popchar(s)))con->val=con->val*10+scaner-'0';
					double jie=0.1;
					if(scaner=='.'){
						while(numberjudge(scaner=popchar(s))){
							con->val+=jie*(scaner-'0');
							jie*=0.1;
						}
					}
					con->unit="";
					if(letterjudge(scaner)){
						con->unit+=scaner;
						while(letterjudge(scaner=popchar(s))){
							con->unit+=scaner;
						}
					}
					expr->data=reinterpret_cast<void*>(con);
					t->push_back(*expr); delete expr;
					continue;
				}
				if(scaner=='#'){
					ExprElement*expr=new ExprElement;
					expr->type=COLOR;
					LessColor*color=new LessColor;
					color->r=color->g=color->b=0;
					int flag2=0;
					if(s->length()>=6)
						if(judge16(s[0][3])&&judge16(s[0][4])&&judge16(s[0][5]))
							flag2=1;
				 	for(int i=0;i<=flag2;i++)rgbread(s,color->r,flag2);
				 	for(int i=0;i<=flag2;i++)rgbread(s,color->g,flag2);					 
				 	for(int i=0;i<=flag2;i++)rgbread(s,color->b,flag2);	
					expr->data=reinterpret_cast<void*>(color);
					t->push_back(*expr);delete expr;
					scaner=popchar(s);
					continue;
				}
				if(scaner=='r'){
					ExprElement*expr=new ExprElement;
					expr->type=COLOR;
					LessColor*color=new LessColor;
					color->r=getnumber(s);
					color->g=getnumber(s);
					color->b=getnumber(s);
					expr->data=reinterpret_cast<void*>(color);
					t->push_back(*expr);delete expr;
					scaner=popchar(s);
					continue;
				}
			}
	}	
	return t;
}
bool checkcom(string*s,string*temp){
	if(s[0][0]=='/'){
		if(s[0][1]=='/'){
			while(s[0][0]!='\n')s->erase(0,1);
				s->erase(0,1);
				return true;
			}
		if(s[0][1]=='*'){
			*temp+="/*";s->erase(0,2);
			while(!((s[0][0]=='*')&&(s[0][1]=='/'))){
				*temp+=s[0][0];s->erase(0,1);
			} 
			*temp+="*/";s->erase(0,2);
			return true;
		}
	}
	return false;
} 
string*read(string*s,char c){
	string*temp=new string;*temp="";
	if(c==';'){
		while(s[0][0]!=';'){
			if(checkcom(s,temp))continue;
			if(s[0][0]=='"'){
				*temp+='"';s->erase(0,1);
				string*t=read(s,'"');
				*temp+=*t;delete t;
				*temp+='"';s->erase(0,1); 
			}
			else if(s[0][0]=='\''){
				*temp+='\'';s->erase(0,1);
				string*t=read(s,'\'');
				*temp+=*t;delete t;
				*temp+='\'';s->erase(0,1); 
			}
			else{
				*temp+=s[0][0];s->erase(0,1);
			}
		}
		return temp;
	}
	if(c=='"'){
		while(1){
			if(checkcom(s,temp))continue;
			if(s[0][0]=='"'){
				if(temp->empty())break;
				if(temp[0][temp->length()-1]!='\\')
					break;
				}
			*temp+=s[0][0];s->erase(0,1);
		}
		return temp;
	}
	if(c=='\''){
		while(1){
			if(checkcom(s,temp))continue;
			if(s[0][0]=='\''){
				if(temp->empty())break; 
				if(temp[0][temp->length()-1]!='\\')
					break;
				} 
			*temp+=s[0][0];s->erase(0,1);
		}
		return temp;
	}
	if(c=='}'){
		int clock=0;
		while((clock!=0)or(s[0][0]!='}')){
			if(checkcom(s,temp))continue;
			if(s[0][0]=='"'){
				*temp+='"';s->erase(0,1);
				string*t=read(s,'"');
				*temp+=*t;delete t;
				*temp+='"';s->erase(0,1); 
				continue;
			}
			if(s[0][0]=='\''){
				*temp+='\'';s->erase(0,1);
				string*t=read(s,'\'');
				*temp+=*t;delete t;
				*temp+='\'';s->erase(0,1); 
				continue;
			}  
			if(s[0][0]=='{')clock++;
			if(s[0][0]=='}')clock--;
			*temp+=s[0][0];s->erase(0,1);
		}
		return temp;
	}	
	if(c==')'){
		int clock=0;
		while((clock!=0)or(s[0][0]!=')')){
			if(checkcom(s,temp))continue;
			if(s[0][0]=='"'){
				*temp+='"';s->erase(0,1);
				string*t=read(s,'"');
				*temp+=*t;delete t;
				*temp+='"';s->erase(0,1); 
				continue;
			}
			if(s[0][0]=='\''){
				*temp+='\'';s->erase(0,1);
				string*t=read(s,'\'');
				*temp+=*t;delete t;
				*temp+='\'';s->erase(0,1); 
				continue;
			} 
			if(s[0][0]=='(')clock++;
			if(s[0][0]==')')clock--;
			*temp+=s[0][0];s->erase(0,1);
		}
		return temp;
	}	 
}
string*getname(string*s,const char c){
	string*name=new string;*name="";
	while(s[0][0]!=c){
		*name+=s[0][0];s->erase(0,1);
	}
	while((name[0][name->length()-1]=='\n')or(name[0][name->length()-1]==' '))name->erase(name->length()-1,1);
	return name;
} 
void mixinparam(vector<Expression>*params,string*s){
	while(!s->empty()){
		string*temps=new string;*temps="";
		while((s[0][0]!=',')&&(s[0][0]!=';')&&(!s->empty())){
			*temps+=popchar(s);
			if(temps[0][temps->length()-1]=='"'){
				string*t=read(s,'"');*temps+=*t;delete t;*temps+=popchar(s);			
			}
			if(temps[0][temps->length()-1]=='\''){
				string*t=read(s,'\'');*temps+=*t;delete t;*temps+=popchar(s);			
			}			
		}
		Expression*expr=getexpr(temps);delete temps;
		params->push_back(*expr);delete expr; 
	}
}
LessParam*getparam(string*s){
		LessParam*t=new LessParam;
		while(s[0][0]!='@')s->erase(0,1);s->erase(0,1);
		*s+=':';
		string*name=getname(s,':');s->erase(0,1);
		t->name=*name;delete name;t->expression.clear();
		if(!s->empty())s->erase(s->length()-1,1);
		if(!s->empty()){
			Expression*tt=getexpr(s);
			t->expression=*tt;delete tt;
		}
	return t;
}
void lessparam(vector<struct LessParam>*params,string*s){
	while(!s->empty()){
		string*temps=new string;*temps="";
		
		while((s[0][0]!=',')&&(s[0][0]!=';')&&(!s->empty())){
			*temps+=popchar(s);
			if(temps[0][temps->length()-1]=='"'){
				string*t=read(s,'"');*temps+=*t;delete t;*temps+=popchar(s);			
			}
			if(temps[0][temps->length()-1]=='\''){
				string*t=read(s,'\'');*temps+=*t;delete t;*temps+=popchar(s);			
			}			
		}
		LessParam*param=getparam(temps);delete temps;
		params->push_back(*param);delete param; 
	}	
} 
LessBlock*read(string*s){
	LessBlock*p=new LessBlock;
	while(!s->empty()){
		if((s[0][0]==' ')or(s[0][0]=='\n')or(s[0][0]==';'))while((s[0][0]==' ')or(s[0][0]=='\n')or(s[0][0]==';'))s->erase(0,1);
		if(s->empty())return p;
		string*ts=new string;*ts="";
		if(checkcom(s,ts)){
			if(!ts->empty()){
				LessElement*t=new LessElement;
				t->type=BLOCK_COMMENT;
				t->data=reinterpret_cast<void*>(ts);
				p->push_back(*t);delete t;
				continue; 
			}
		}
		delete ts;
		int x,y;
		for(int i=0;i<=s[0][s->length()-1];i++){
			if(s[0][i]=='('){
				x=i;y=1;break;
			}
			if(s[0][i]=='{'){
				if(i==0){
					x=i;y=2;break;
				}
				if(s[0][i-1]!='@'){
					x=i;y=2;break;
				} 	
			}
			if(s[0][i]==':'){
				x=i;y=3;break;
			}
		}
		switch(y){
			case 1:{
				LessElement*t=new LessElement;
				string*name=getname(s,'(');s->erase(0,1);
				string*param=read(s,')');s->erase(0,1);
				while((s[0][0]==' ')or(s[0][0]=='\n'))s->erase(0,1);
				if(s[0][0]=='{'){			
					s->erase(0,1);
					string*temps=read(s,'}');s->erase(0,1);*temps+=';';
					LessBlock*tempb=read(temps);delete temps;
					LessSelector*temp=new LessSelector;
					temp->selector_body=*tempb;delete tempb;	
					vector<struct LessParam>*params=new vector<struct LessParam>;
					lessparam(params,param);delete param;	
					temp->name=*name;temp->params=*params;
					delete name;delete param;
					t->type=PARAMETRIC_SELECTOR;				
					t->data=reinterpret_cast<void*>(temp);
					p->push_back(*t);delete t;
				}
				else{
					t->type=MIXIN;
					LessMixin*temp=new LessMixin;
					vector<Expression>*params=new vector<Expression>;
					mixinparam(params,param);delete param;	
					temp->name=*name;temp->params=*params;
					delete name;delete param;
					t->data=reinterpret_cast<void*>(temp);
					p->push_back(*t);delete t;
				}
			}
			case 2:{
				LessElement*t=new LessElement;
				t->type=NORMAL_SELECTOR;
				LessSelector*temp=new LessSelector;
				string*name=getname(s,'{');s->erase(0,1);
				temp->name=*name;delete name;			
				temp->params.clear();
				string*temps=read(s,'}');s->erase(0,1);*temps+=';';
				LessBlock*tempb=read(temps);delete temps;
				temp->selector_body=*tempb;delete tempb;
				t->data=reinterpret_cast<void*>(temp);
				p->push_back(*t);delete t;

			}
			case 3:{
				bool flag=0;
				if(s[0][0]=='@')
					if(s[0][1]!='{'){flag=1;s->erase(0,1);}
				string*name=getname(s,':');s->erase(0,1);
				string*temps=read(s,';');s->erase(0,1);
				LessElement*t=new LessElement;
				if(flag){
					LessDef*temp=new LessDef; 
					temp->name=*name;
					Expression*tt=getexpr(temps);
					temp->expression=*tt;delete tt; 
					t->data=reinterpret_cast<void*>(temp);
					t->type=DEF;
				}
				else{
					LessCssRule*temp=new LessCssRule;
					temp->name=*name;
					Expression*tt=getexpr(temps);
					temp->expression=*tt;delete tt;
					t->data=reinterpret_cast<void*>(temp);
					t->type=CSS_RULE;
				}
				delete name;delete temps;
				p->push_back(*t);delete t;
			}	
		}
	}
	return p;	
}
int main(){
	LessBlock*p=new LessBlock;string s="";char c;
	while(c=getc(stdin))s+=c;s+=';';
	p=read(&s);
	return 0;
} 
