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
	COLOR,
	LEFT_BRACE,
	RIGHT_BRACE,
	OP_AT,
	OP_ADD,
	OP_NEG,
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
struct LessParam;
struct LessSelector;
struct LessMixin;
struct LessCssRule;

typedef list<struct LessElement> LessBlock;
typedef vector<struct ExprElement> Expression;

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

struct LessParam {
	string name;
	Expression expression;
};

struct LessColor {
	int r;
	int g;
	int b;

        static void rim(struct LessColor &color) {
                if(color.r >= 255) color.r = 255;
                if(color.b >= 255) color.b = 255;
                if(color.g >= 255) color.g = 255;
                if(color.r < 0) color.r = 0;
                if(color.g < 0) color.g = 0;
                if(color.b < 0) color.b = 0;
        }

};

struct LessSelector {
	string	name;
	vector<struct LessParam> params;
	LessBlock selector_body;
};

struct LessMixin {
	string name;
	vector<Expression> params;
};

struct LessCssRule {
	string name;
	Expression expression;
};

#endif
