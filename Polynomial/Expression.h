#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_
#define MAXVAR 26
#include <stdio.h>

/*可以更改typedef 为double来支持小数*/
typedef int PolyType;
/*Note that PolyVarType is an integer denoting its column in the varible table.
* In order to sort variables, one just have to compare its value.
*/
typedef int PolyVarType;

typedef struct poly_poly
{
	PolyType degree;
	PolyVarType var;
	struct poly_poly *son;
} Poly;

typedef struct exp_exp
{
	PolyType coeff;
	struct exp_exp *next;
	struct poly_poly *son;
} Expressions;

extern int ExpressionSize;
extern int PolySize;

extern char varTable[MAXVAR];
extern bool varValueConfig[MAXVAR];
extern PolyType varValue[MAXVAR];
void initVar();
int isVarInTable(char c);

//it sorts the expression
void expressionSorter(Expressions *&exp);

void freePolyList(Poly *&head);
void freeExpression(Expressions *&head);

/**
* @brief: add a new variable to certain expression
* @param [in] var: the variable to be added
* @param [in] exp: expression where var to be added
* @exception
* @return 0: end successfully
* @note: it is designed specifically for Expressions *newExpression
* it will automatically add to the varTable if var doesn't exists
*/
int addNewVariable(char var, Expressions *exp, PolyVarType degree = 1);
/**
* @brief: create new expression according to the string
* @param [in] s: a pointer to the string to be translated
* @exception
* @return :the pointer to the head of the expression
* @note: it ends whenever *s is of different kind against *(s-1)
*/
Expressions *newExpression(const char *s);


Poly *polyDuplicate(Poly *Source);
Expressions *expressionDuplicate(Expressions *Source);

#endif // !_EXPRESSION_H_

