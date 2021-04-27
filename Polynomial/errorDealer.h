#pragma once
#include <csignal>
#include <iostream>
#include <csetjmp>
#include <ctime>
#include "ExpressionSet.h"
#include "PolishReverse.h"

extern std::jmp_buf jmp_to_begin;

void setSignalHandler();
/**
* @brief: clean up the mess when the calculation stops half way
* @param [in] ifRegister: whether to register stuff to be cleaned up, if is false, then it will try to clean up
* @exception
* @note: 
*/
void cleanUpHalfTime(bool ifRegister = false, 
	Expressions **exp1 = NULL, 
	Expressions **exp2 = NULL, 
	Expressions **exp_temp =NULL, 
	Expressions **exp3 =NULL, 
	ExpressionSets **expStack = NULL);

void cleanUp(bool ifRegister = false, 
	OperatorStack *op = NULL, 
	FragmentStack *frag = NULL);