#ifndef SSEL_COMMONS_H
#define SSEL_COMMONS_H
#include <list>
#include <vector>
#include <string>

using namespace std;

enum LessElementType {
	DEF,
	NORMAL_SELECTOR,
	PARAMETRIC_SELECTOR,
	BLOCK_COMMENT,
	MIXIN,
	CSS_RULE
};

enum ExprElementType {
	CONSTANT,
	STRING,
	VARIABLE,
	COLOR,
	LEFT_BRACE,
	RIGHT_BRACE,
	OP_AT, 
	OP_ADD,
	OP_SUB,
	OP_MUL,
	OP_DIV,
};

typedef list<struct LessElement> LessBlock; 
typedef vector<struct ExprElement> Expression;

struct LessElement;
struct ExprElement;
struct LessConstant;
struct LessColor;
struct LessDef;
struct Param;
struct LessSelector;
struct LessMixin;
struct LessCssRule;

struct LessElement{
	LessElementType type;
        void *data;
};

struct ExprElement {
	ExprElementType type;
        void *data;
};

struct LessConstant {
	double val;
	string unit;
};

struct LessDef {
	string name;
	Expression expression;
};

struct Param {
	string name;
	Expression expression;
};

struct LessColor {
	int r;
	int g;
	int b;
}

struct LessSelector {
	string	name;
	vector<struct Param> params;
	LessBlock selector_body;
};

struct LessMixin {
	string name;
	vector<Expression> expr;
};

struct LessCssRule {
	string name;
	Expression expression;
};
#endif
