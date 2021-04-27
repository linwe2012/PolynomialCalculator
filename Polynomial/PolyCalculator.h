#ifndef _POLYCALCULATOR_H_
#define _POLYCALCULATOR_H_
#include "ExpressionSet.h"
#include "PolishReverse.h"
/**
* @brief: add expression2 to expresion1
* @param [in] exp1: a pointer to a lists of expression, it will store the result;
* @param [in] exp2: a pointer to a lists of expression, it will be freed;
* @exception
* @note
*/

void add(Expressions* &exp1, Expressions* &exp2);

/**
* @brief: subtract expression2 from expresion1
* @param [in] exp1: a pointer to a lists of expression, it will store the result;
* @param [in] exp2: a pointer to a lists of expression, it will be freed;
* @exception
* @note
*/
void sub(Expressions* &exp1, Expressions* &exp2);

/**
* @brief: add expression2 to expresion1
* @param [in] exp1: a pointer to a lists of expression, it will store the result;
* @param [in] exp2: a pointer to a lists of expression, it will be freed;
* @exception
* @note
*/
Expressions * mul(Expressions *exp1, Expressions *exp2);
/**
* @brief: compare p1's priority against p2
* @param [in] p1: a pointer to a lists of expression
* @param [in] p2: a pointer to a lists of expression
* @return 0: the expressions are the same(should be merged)
* @return 1: p1 has a higher priority
* @return -1: p2 has a higher priority
* @exception
* @note: it doesn't rely on a list of priority, it compares var cuz var itself is a sign of priority 
*/
int compare(Expressions* p1, Expressions* p2);
void expressionZeroEliminator(Expressions *&expHead);
int analysisString(char *s);
void initialCongratulate();
int cal(FragmentStack *frag, ExpresionBuffer *expb);
#endif // !_POLYCALCULATOR_H_

