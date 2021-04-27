#include "errorDealer.h"
#include "IOControl.h"
#include "command.h"
#include <Windows.h>

std::jmp_buf jmp_to_begin;

void cleanUp(bool ifRegister, OperatorStack *op, FragmentStack *frag) 
{
	static OperatorStack *cl_op;
	static FragmentStack *cl_frag;
	if (ifRegister) {
		cl_op = op;
		cl_frag = frag;
	}
	else {
		cleanUpHalfTime();
		deleteFragmenStack(frag);
		deleteOperatorStack(op);
	}
}

void cleanUpHalfTime(bool ifRegister, Expressions **exp1, Expressions **exp2, Expressions **exp_temp, Expressions **exp3, ExpressionSets **expStack)
{
	static Expressions **cl_exp1, **cl_exp2, **cl_exp_temp, **cl_exp3;
	static ExpressionSets **cl_expStack;
	if (ifRegister) {
		cl_exp1 = exp1;
		cl_exp2 = exp2;
		cl_exp_temp = exp_temp;
		cl_exp3 = exp3;
		cl_expStack = expStack;
	}
	else
	{
		if (*cl_exp1 != NULL) {
			freeExpression(*cl_exp1);
		}
		if (*cl_exp2 != NULL) {
			freeExpression(*cl_exp2);
		}
		if (*cl_exp3 != NULL) {
			freeExpression(*cl_exp3);
		}
		if (*cl_exp_temp != NULL) {
			freeExpression(*cl_exp_temp);
		}
		if (cl_expStack != NULL) {
			freeExpressionSets(*cl_expStack);
		}
	}
}

void signalHander(int signum)
{
	static int sigSegvErrorAmount = 0;
	static bool sigsegv = false;
	static time_t errorTime1;
	static time_t errorTime2;
	static bool timeSwitch = false;

	switch (signum)
	{
	case SIGTERM:
	{
		cleanUp();
		std::cerr << "signalHander::Clean-up is finished, the program will end." << std::endl;
		exit(0);
		break;
	}
	case SIGFPE: 
	{
		std::cerr << "signalHander::Erroneous arithmetic operation, please input another expression." << std::endl;
		cleanUpHalfTime();
		longjmp(jmp_to_begin, SIGFPE);
		break;
	}
	case SIGSEGV:
	{
		if (timeSwitch) {
			time(&errorTime1);
		}
		else {
			time(&errorTime2);
		}

		if (difftime(errorTime1, errorTime2) < cmd_ErrorTimeGap) {
			sigSegvErrorAmount++;
		}

		if (sigSegvErrorAmount > cmd_errorOccurance) {
			std::cerr << "signalHander::There might be some errors with clean-up functions, the program will ends" << std::endl;
			getchar(); getchar();
			exit(SIGSEGV);
		}
		else {
			sigSegvErrorAmount = 0;
		}
		std::cerr << "signalHander::Invalid memory access, please initiate an issue at Github." << std::endl;
		if (!sigsegv) {
			ShellExecute(NULL, "open", "https://github.com/linwe2012/CProject/tree/master/Polynomial", NULL, NULL, SW_MINIMIZE);
			sigsegv = true;
		}
		
		cleanUpHalfTime();
		longjmp(jmp_to_begin, SIGSEGV);
		break;
	}
	default:
		break;
	}
}

void setSignalHandler()
{
	signal(SIGTERM, signalHander);
	signal(SIGFPE, signalHander);
	signal(SIGSEGV, signalHander);
}