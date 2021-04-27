#ifndef _COMMAND_H_
#define _COMMAND_H_
#define CMD_TRUE 1
#define CMD_FALSE 0
#define CMD_DEFAULT_TIME_LIMIT 6
#define GREY 8
#define WHITE 7
#define LIGHT_WHITE 0xF
#define RED 4
#define BLUE 1
#define LIGHT_BLUE 9
#define YELLOW 6
#define MAX_CMD 25
#include "ExpressionSet.h"
#include "PolishReverse.h"

extern int cmd_color;
extern int cmd_autoCorrect;
extern int cmd_autoParenthese;
extern int cmd_maxErrorLog;
extern int cmd_autoSave;
extern int cmd_TimeLimit;
extern double cmd_ErrorTimeGap;
extern int cmd_errorOccurance;

int initCommand();
int cmdDealer(char *s, ExpresionBuffer *expb, FragmentStack *frag, OperatorStack *opStack, char *bufferHead);
void throwError(const char*errorLog, int color);
void cmdDraw(PolyVarType x, PolyVarType y, PolyType step, PolyType range, PolyType offset_x, PolyType offset_y, ExpressionSets *exps);
/**
* @brief: translate a string to a certain type of number
* @param [in] s:the string
* @exception
* @return: result
* @note: it will change the input string pointer, therefore you can't input names' of array
* it will start from the position one step BEFORE the number
* it will stop whenever meets non-number character
* it will also move one step forward, where the first non-number char lies
*/
template <typename T>
T cmdGetNum(char *&s)
{
	T res = 0;
	int flag = 1;
	T denominator = 10;
	if (*s != '\0' && *s != '\n') {
		s++;
		if (*s == '-') {
			flag = -flag;
		}
		res = 0;
		while (*s >= '0' && *s <= '9') {
			res = res * 10 + *s - '0';
			s++;
		}
		if (*s == '.')
		{
			s++;
			while (*s >= '0' && *s <= '9') {
				res = res + (*s - '0') / denominator;
				s++;
				denominator *= 10;
			}
		}
	}
	return T(flag * res);
}

template <typename T1>
T1 cmdGetNum_static(const char *s)
{
	T1 res = 0;
	int flag = 1;
	T1 denominator = 10;
	if (*s != '\0' && *s != '\n') {
		s++;
		if (*s == '-') {
			flag = -flag;
		}
		res = 0;
		while (*s >= '0' && *s <= '9') {
			res = res * 10 + *s - '0';
			s++;
		}
		if (*s == '.')
		{
			s++;
			while (*s >= '0' && *s <= '9') {
				res = res + (*s - '0') / denominator;
				s++;
				denominator *= 10;
			}
		}
	}
	return T1(flag * res);
}

/**
* @brief: it will read from string to get values of variable and calculate
* @param [in] exps: the expression sets to be calculated
* @param [in] expb: the expression buffer in which the result will be stored;
* @param [in] s: the string
* @exception
* @return: result
* @note: it will duplicate a brand-new expresion set for calculation
* the result will be pushed into buffer
* it will print automatically
* it won't try to mess with the varTable cuz it doesn't seem to matter when extra var exists
* and it won't cause any trouble with the var, which would be changed according to the varTable
* it has a strange name cuz I don't know how to call it
*/
int varValueControl(ExpressionSets *exps, ExpresionBuffer *expb, char *s);

#endif // !_COMMAND_H_
