#ifndef _EXPRESSIONSET_H_
#define _EXPRESSIONSET_H_
#include "Expression.h"

#define EXPRESSION_MAX_BUFFER 5
#define EXPRESSION_SETS_INI_SIZE 1
#define EXPRESSION_SETS_INCRETMENT 2

#define DEFAULT_BUFFER_FILE_ADDRESS "./res/buffer.exp"

#define MAX_NAME 20

typedef Expressions* ExpressionSetsElem;
typedef struct {
	ExpressionSetsElem *base;
	ExpressionSetsElem *top;
	int maxSize;
	int ID;
	char name[MAX_NAME];
	char varTable[MAXVAR];
}ExpressionSets;

typedef ExpressionSets* ExpresionBufferElem;
typedef struct {
	ExpresionBufferElem *base;
	ExpresionBufferElem *top;
	int maxSize;
	int currentID;
	int offset;
}ExpresionBuffer;



extern int globe_ID;
extern int expressionSetsSize;
extern int expressionBufferSize;

ExpressionSets *initExpressiosSets(bool hasID, ExpressionSets *exps, int initSize = EXPRESSION_SETS_INI_SIZE);
void pushExpressiosSets(ExpressionSets *exps, ExpressionSetsElem exp);
ExpressionSetsElem popExpressiosSets(ExpressionSets *exps, ExpressionSetsElem *exp = NULL);
void freeExpressionSets(ExpressionSets *&exps);
ExpressionSets *expressionSetsDuplicate(ExpressionSets *Source, bool hasID = true);
void pushNumberSignToExpression(const char *s, ExpressionSets *exps, ExpresionBuffer *expb);

/**
* @brief: calculate all the expressions into a number
* @param [in] exps: the expression sets to be calculated
* @exception
* @note: all the variable's value will be read from globle varValue[];
* it will not check if the value has any value
*/
PolyType caluclateExpressionResult(ExpressionSets *exps);

/**
* @brief: calculate the expression sets and merge 
* @param [in] exps: the expression sets to be calculated
* @exception
* @note: all the variable's value will be read from globle varValue[];
* it will check if the value has any value from varValueConfig[];
*/
void caluclateExpression(ExpressionSets *exps);

void clearVarValue();
void clearVarTable();

void initExpressionBuffer(ExpresionBuffer *expb);
/**
* @brief: push a expresion set in buffer
* @param [in] expb: a pointer to expression buffer;
* @param [in] ifSave:save to file or not
* @exception 
* @return
* @note: if the stack is full, then it will clear off;
*/
void pushExpressionBuffer(ExpresionBuffer *expb, ExpresionBufferElem exps, bool ifSave);

/**
* @brief: print the Expression Set
* @exception
* @return
* @note: only print the base of it
*/
ExpressionSets *getCurrentBufferExpressionSets(ExpresionBuffer *expb);


void printExpressionSet(ExpressionSets *exps);

/**
* @brief: print the latest expression set added in to expression buffer
* @param [in] expb: a pointer to expression buffer;
* @exception
* @return
* @note:
*/
void printBufferCurrentOffset(ExpresionBuffer *expb);
void expressionBufferOffsetUpdate(ExpresionBuffer *expb, int ID);
void expressionBufferIDReset(ExpresionBuffer *expb);

void printExpression(Expressions *exp);
void printExpressionSet_all(ExpressionSets *exps);
void printfAllBuffer(ExpresionBuffer *expb);

void expressionSetsVarCpy(ExpressionSets *exps);
void expressionSetsVarSync(ExpressionSets *exps);

inline bool isLegalExpressionSetNameChar(char c);
inline char getRandomLetter();
int nameExpressionSet(ExpressionSets *exps, char *name);
void nameLegalizeCpy(char *destination, const char *source);

ExpressionSets *getExpressionSet(int ID, ExpresionBuffer *expb, bool ifDuplicte, int *ifFromFile = NULL, char *name = NULL);

void printExpressionSet_ID(ExpresionBuffer *expb);
int printExpressionBuffer(ExpresionBuffer *expb);
bool checkExpressionBufferID(ExpresionBuffer *expb, int ID);

#endif // !_EXPRESSIONSET_H_

