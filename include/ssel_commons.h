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

struct LessColor operator+(struct LessColor &a, struct LessConstant &b) {
        struct LessColor result = a;
        result.r += b.val;
        result.g += b.val;
        result.b += b.val;
        LessColor::rim(result);
        return result;
}
struct LessColor operator-(struct LessColor &a, struct LessConstant &b) {
        struct LessColor result = a;
        result.r -= b.val;
        result.g -= b.val;
        result.b -= b.val;
        LessColor::rim(result);
        return result;
}
struct LessColor operator*(struct LessColor &a, struct LessConstant &b) {
        struct LessColor result = a;
        result.r *= b.val;
        result.g *= b.val;
        result.b *= b.val;
        LessColor::rim(result);
        return result;
}

struct LessColor operator+(struct LessConstant &a, struct LessColor &b) {
        return (b+a);
}
struct LessColor operator-(struct LessConstant &a, struct LessColor &b) {
        return (b-a);
}
struct LessColor operator*(struct LessConstant &a, struct LessColor &b) {
        return (b*a);
}

struct LessColor operator/(struct LessColor &a, struct LessConstant &b) {
        struct LessColor result = a;
        result.r /= b.val;
        result.g /= b.val;
        result.b /= b.val;
        LessColor::rim(result);
        return result;
}

struct LessColor operator+(struct LessColor &a, struct LessColor &b) {
        struct LessColor result = a;
        result.r += b.r;
        result.g += b.g;
        result.b += b.b;
        LessColor::rim(result);
        return result;
}
struct LessColor operator-(struct LessColor &a, struct LessColor &b) {
        struct LessColor result = a;
        result.r -= b.r;
        result.g -= b.g;
        result.b -= b.b;
        LessColor::rim(result);
        return result;
}
struct LessColor operator*(struct LessColor &a, struct LessColor &b) {
        struct LessColor result = a;
        result.r *= b.r;
        result.g *= b.g;
        result.b *= b.b;
        LessColor::rim(result);
        return result;
}
struct LessColor operator/(struct LessColor &a, struct LessColor &b) {
        struct LessColor result = a;
        result.r /= b.r;
        result.g /= b.g;
        result.b /= b.b;
        LessColor::rim(result);
        return result;
}
struct LessConstant operator+(struct LessConstant &a, struct LessConstant &b)
{
        struct LessConstant result;
        result = a;
        result.val += b.val;
        if(a.unit != "") {
                result.unit = a.unit;
        } else {
                result.unit = b.unit;
        }

        return result;
}
struct LessConstant operator-(struct LessConstant &a, struct LessConstant &b)
{
        struct LessConstant result;
        result = a;
        result.val -= b.val;
        if(a.unit != "") {
                result.unit = a.unit;
        } else {
                result.unit = b.unit;
        }

        return result;
}
struct LessConstant operator*(struct LessConstant &a, struct LessConstant &b)
{
        struct LessConstant result;
        result = a;
        result.val *= b.val;
        if(a.unit != "") {
                result.unit = a.unit;
        } else {
                result.unit = b.unit;
        }

        return result;
}
struct LessConstant operator/(struct LessConstant &a, struct LessConstant &b)
{
        struct LessConstant result;
        result = a;
        result.val += b.val;
        if(a.unit != "") {
                result.unit = a.unit;
        } else {
                result.unit = b.unit;
        }

        return result;
}

struct ExprElement operator+(struct ExprElement &a, struct ExprElement &b) {
        struct ExprElement result;
        if(a.type == COLOR) {
                result.type = COLOR;
                result.data = new LessColor;

                if(b.type == COLOR) {
                        *((struct LessColor*)result.data) = *((struct LessColor *)a.data) + *((struct LessColor *)a.data);
                } else {
                        *((struct LessColor*)result.data) = *((struct LessColor *)a.data) + *((struct LessConstant *)a.data);
                }
        } else {
                if(b.type == COLOR) {
                        result.type = COLOR;
                        result.data = new LessColor;
                        *((struct LessColor*)result.data) = *((struct LessConstant *)a.data) + *((struct LessColor *)a.data);
                } else {
                        result.type = CONSTANT;
                        result.data = new LessConstant;
                        *((struct LessConstant*)result.data) = *((struct LessConstant *)a.data) + *((struct LessConstant *)a.data);
                }
        }

        return result;
}
struct ExprElement operator-(struct ExprElement &a, struct ExprElement &b) {
        struct ExprElement result;
        if(a.type == COLOR) {
                result.type = COLOR;
                result.data = new LessColor;

                if(b.type == COLOR) {
                        *((struct LessColor*)result.data) = *((struct LessColor *)a.data) - *((struct LessColor *)a.data);
                } else {
                        *((struct LessColor*)result.data) = *((struct LessColor *)a.data) - *((struct LessConstant *)a.data);
                }
        } else {
                if(b.type == COLOR) {
                        result.type = COLOR;
                        result.data = new LessColor;
                        *((struct LessColor*)result.data) = *((struct LessConstant *)a.data) - *((struct LessColor *)a.data);
                } else {
                        result.type = CONSTANT;
                        result.data = new LessConstant;
                        *((struct LessConstant*)result.data) = *((struct LessConstant *)a.data) - *((struct LessConstant *)a.data);
                }
        }

        return result;
}
struct ExprElement operator*(struct ExprElement &a, struct ExprElement &b) {
        struct ExprElement result;
        if(a.type == COLOR) {
                result.type = COLOR;
                result.data = new LessColor;

                if(b.type == COLOR) {
                        *((struct LessColor*)result.data) = *((struct LessColor *)a.data) * (*((struct LessColor *)a.data));
                } else {
                        *((struct LessColor*)result.data) = *((struct LessColor *)a.data) * (*((struct LessConstant *)a.data));
                }
        } else {
                if(b.type == COLOR) {
                        result.type = COLOR;
                        result.data = new LessColor;
                        *((struct LessColor*)result.data) = *((struct LessConstant *)a.data) * (*((struct LessColor *)a.data));
                } else {
                        result.type = CONSTANT;
                        result.data = new LessConstant;
                        *((struct LessConstant*)result.data) = *((struct LessConstant *)a.data) * (*((struct LessConstant *)a.data));
                }
        }

        return result;
}
struct ExprElement operator/(struct ExprElement &a, struct ExprElement &b) {
        struct ExprElement result;
        if(a.type == COLOR) {
                result.type = COLOR;
                result.data = new LessColor;

                if(b.type == COLOR) {
                        *((struct LessColor*)result.data) = *((struct LessColor *)a.data) / *((struct LessColor *)a.data);
                } else {
                        *((struct LessColor*)result.data) = *((struct LessColor *)a.data) / *((struct LessConstant *)a.data);
                }
        } else {
                result.type = CONSTANT;
                result.data = new LessConstant;
                *((struct LessConstant*)result.data) = *((struct LessConstant *)a.data) / *((struct LessConstant *)a.data);
        }

        return result;
}

#endif
