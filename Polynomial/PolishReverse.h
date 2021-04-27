#ifndef _POLISHREVERSE_H_
#define _POLISHREVERSE_H_

#define EMPTY_OPERATOR 0
typedef const char* OperatorType;
typedef struct
{
	OperatorType *base;
	OperatorType *top;
	int stackSize;
}OperatorStack;

typedef const char* FragmentType;
typedef struct
{
	FragmentType *base;
	FragmentType *top;
	int stackSize;
}FragmentStack;

const char autoMultiplyOperator[2] = "*";
//if a minus operater is a unary
const char negationIdentifier[2] = "~";

inline bool isNumber(char c);
inline bool isOperator(char c);
inline bool isVariable(char c);
inline bool isStringEnd(char c);
/**
* @brief: disassemble string into stacks of pointers to the first of the fragment
* @param [in] s: string to be disassembled
* @param [in] op: operator Stack where operators are stored
* @param [in] frag: fragment Stack where fragments(disassembled string) are stored
* @exception
* @note: 
*/
void RPN(char *s, OperatorStack *op, FragmentStack *frag);

void initOperatorStack(OperatorStack *s);
void clearOperatorStack(OperatorStack *s);
void deleteOperatorStack(OperatorStack *s);

void initFragmentStack(FragmentStack *s);
void clearFragmentStack(FragmentStack *s);
void deleteFragmenStack(FragmentStack *s);


void printRPN(FragmentStack *frag, char *head);

#endif // !_POLISHREVERSE_H_
