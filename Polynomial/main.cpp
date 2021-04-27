#include <stdio.h>
#include <windows.h>
#include <math.h>
//#include <pthread.h>
#include "PolishReverse.h"
#include "IOControl.h"
#include "command.h"
#include "ExpressionSet.h"
#include "PolyCalculator.h"
#include "initializer.h"
#include "errorDealer.h"

//#pragma comment(lib,"pthreadVC2.lib") 

//struct parameter {
//	FragmentStack *frag;
//	ExpresionBuffer *expb;
//} para;
//(x+y)(x-y)(a+b+c)

//void thread1(struct parameter *para);

int main()
{
	OperatorStack opStack;
	FragmentStack fragStack;
	ExpresionBuffer expBuffer;

	//ExpressionSets *exps1;
	ExpressionSets *exps;
	//Expressions *exp1;
	
	char buffer[MAX_BUFFER];
	char tempBuffer[MAX_BUFFER];
	int i = 0, ifFromFile;

	//pthread_t thd1;
	SetConsoleTitle("Polynomial Calculator");

	initCommand();
	
	initExpressionBuffer(&expBuffer);

	initFragmentStack(&fragStack);
	initOperatorStack(&opStack);

	initIOControl();

	cleanUp(true, &opStack, &fragStack);
	//setMyCurrentConsolefont();
	setSignalHandler();
	if (iniHello == 0) {
		firstTimeWelcome(&opStack, &fragStack, &expBuffer);
		iniHello = 1;
	}
	else {
		initialWelcome();
	}

	setjmp(jmp_to_begin);

	while (1) {
		fgets(tempBuffer, MAX_BUFFER, stdin);
		int bufferLength = strlen(tempBuffer);
		if (bufferLength == 0) continue;
		if (*tempBuffer == '\\') {
			i = cmdDealer(tempBuffer + 1, &expBuffer, &fragStack, &opStack, buffer);
			if (i >= 0) {
				exps = getExpressionSet(i, &expBuffer, false, &ifFromFile);
				expressionSetsVarSync(exps);
				if (ifFromFile == 1) {
					pushExpressionBuffer(&expBuffer, exps, false);
					expressionBufferOffsetUpdate(&expBuffer, i);
				}
				else {
					expressionBufferOffsetUpdate(&expBuffer, i);
				}
				printBufferCurrentOffset(&expBuffer);
			}
		}
		else {
			compactArray(tempBuffer, ' ');
			bufferLength = compactArray(tempBuffer, '\n');
			if (bufferLength == 0) continue;
			if (*(tempBuffer + 1) == '=') {
				//printf("%s", tempBuffer);
				varValueControl(getCurrentBufferExpressionSets(&expBuffer), &expBuffer, tempBuffer);
			}
			else {
				strcpy_s(buffer, tempBuffer);
				if (analysisString(buffer) == 0) {
					clearOperatorStack(&opStack);
					clearFragmentStack(&fragStack);
					clearVarValue();
					clearVarTable();

					RPN(buffer, &opStack, &fragStack);
					//printRPN(&fragStack, buffer);

					cal(&fragStack, &expBuffer);
					//exps1 = getCurrentBufferExpressionSets(&expBuffer);
					//exp1 = expressionDuplicate(*(exps1->base));
					//printExpression(exp1);
					//printBufferCurrentOffset(&expBuffer);
					printf("\n");
				}
			}
			
		}
	}

	return 0;
}


//void thread1(struct parameter *para)
//{
//	cal(para->frag, para->expb);
//}