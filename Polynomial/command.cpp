#include "command.h"
#include "IOControl.h"
#include "ExpressionSet.h"
#include "PolishReverse.h"
#include "initializer.h"
#include "errorDealer.h"
#include <string.h>
#include <stdio.h>
#include <Windows.h>
#include <math.h>
#include <direct.h>
#define CMD_RESPONSE_COLOR GREY
#define CMD_TEMP_MAX_BUFFER 6
#define CMD_HELP_KINDS 13
#define M_PI 3.14159265358979323846

int cmd_color = CMD_TRUE;
int cmd_autoCorrect= CMD_TRUE;
int cmd_maxErrorLog = 4;
int cmd_autoParenthese = CMD_TRUE;
int cmd_autoSave = CMD_TRUE;
int cmd_TimeLimit = CMD_DEFAULT_TIME_LIMIT;
double cmd_ErrorTimeGap = 1;
int cmd_errorOccurance = 2;

void printfSettings(char *currentDir);

char cmd_list[][20] = {
	"\\ac on\\off",  //auto correct
	"\\ap on\\off", 
	"\\color on\\off",
	"\\err num",      //max errors
	"\\u (num)",    //trace back in history
	"\\d (num)",
	//"\\loc num",     //locate at a expression
	"\\sav name",    //save the result
	//"\\load name",
	//"\\read address name",
	"\\shw set",     //show current settings
	"\\shw buf",    //show current buffer
	"\\shw rpn",    //show prn
	"\\ttr",
	"\\clr all",
	"\\clr buf",
	"\\clr sav",
	"\\exit",
};

char cmd_explain[][100] = {
	"turn on or off auto corrector",
	"turn on or off auto parenthese adder",
	"use color or not, if turned off, there may be a higher contrast",
	"determine how many errors a function can throw. Certain functions apply",
	"trace back in history",
	"trace down in history, if the number is ahead of time, it will use the latest one",
	//"locate a certain expression",
	"save the expression with its name",
	//"load expression saved",
	//"read expression",
	"show current settings",
	"show current buffer",
	"show the result of Reverse Polish notation",
	"start tutorial",
	"clear all the files",
	"clear the buffer file",
	"clear file that saves the name of the expression",
	"exit the program",
};
void printHelp();
int cmdFragmentCopy(char *Destination, char *origin, int maxLength);


int cmdDealer(char *s, ExpresionBuffer *expb, FragmentStack *frag,OperatorStack *opStack ,char *bufferHead) {
	char pre_cmd[CMD_TEMP_MAX_BUFFER];
	char cmd_speci[CMD_TEMP_MAX_BUFFER];
	char name[MAX_NAME]= "";
	static char *mdirBuffer = _getcwd(NULL, 0);
	int offset;
	int res = -1;
	int currentID = expb->currentID;
	char *ptr = s;
	ExpressionSets *mexps;
	while (*s == '\\') {
		s++;
	}
	offset = cmdFragmentCopy(pre_cmd, s, CMD_TEMP_MAX_BUFFER);
	if (strcmp(pre_cmd, "help") ==  0) {
		printHelp();
	}
	else if (strcmp(pre_cmd, "ac") == 0) {
		if (*(s + offset) != '\0') {
			cmdFragmentCopy(cmd_speci, s + offset + 1, CMD_TEMP_MAX_BUFFER);
			if (strcmp(cmd_speci, "on") == 0) {
				cmd_autoCorrect = CMD_TRUE;
				throwError("cmdDealer::Auto correcter is turned on.\n", CMD_RESPONSE_COLOR);
			}
			else if (strcmp(cmd_speci, "off") == 0) {
				cmd_autoCorrect = CMD_FALSE;
				throwError("cmdDealer::Auto correcter is turned off.\n", CMD_RESPONSE_COLOR);
			}
		}
	}
	else if (strcmp(pre_cmd, "ap") == 0) {
		if (*(s + offset) != '\0') {
			cmdFragmentCopy(cmd_speci, s + offset + 1, CMD_TEMP_MAX_BUFFER);
			if (strcmp(cmd_speci, "on") == 0) {
				cmd_autoParenthese = CMD_TRUE;
				throwError("cmdDealer::Auto parenthese adder is turned on.\n", CMD_RESPONSE_COLOR);
			}
			else if (strcmp(cmd_speci, "off") == 0) {
				cmd_autoParenthese = CMD_FALSE;
				throwError("cmdDealer::Auto parenthese adder is turned off.\n", CMD_RESPONSE_COLOR);
			}
		}
	}
	else if (strcmp(pre_cmd, "color") == 0) {
		if (*(s + offset) != '\0') {
			cmdFragmentCopy(cmd_speci, s + offset + 1, CMD_TEMP_MAX_BUFFER);
			if (strcmp(cmd_speci, "on") == 0) {
				cmd_color = CMD_TRUE;
				throwError("cmdDealer::Color is allowed.\n", CMD_RESPONSE_COLOR);
			}
			else if (strcmp(cmd_speci, "off") == 0) {
				cmd_color = CMD_FALSE;
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHT_WHITE);
				throwError("cmdDealer::Color is filtered.\n", CMD_RESPONSE_COLOR);
			}
				
		}
	}
	else if (strcmp(pre_cmd, "err") == 0) {
		ptr = s + offset;
		if (*ptr != '\0' && *ptr != '\n') {
			ptr++;
			cmd_maxErrorLog = 0;
			while (*ptr >= '0' && *ptr <= '9') {
				cmd_maxErrorLog = cmd_maxErrorLog * 10 + *ptr - '0';
				ptr++;
			}
			if (cmd_color == CMD_TRUE)
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CMD_RESPONSE_COLOR);
			printf("cmdDealer::max error is set to %d\n", cmd_maxErrorLog);
			if (cmd_color == CMD_TRUE)
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
		}
	}
	else if (strcmp(pre_cmd, "u") == 0 || strcmp(pre_cmd, "d") == 0) {
		ptr = s + offset;
		//res = 1;
		/*
		if (*ptr != '\0' && *ptr != '\n') {
			ptr++;
			res = 0;
			while (*ptr >= '0' && *ptr <= '9') {
				res = res * 10 + *ptr - '0';
				ptr++;
			}
		}*/
		res = cmdGetNum<int>(ptr);
		if (res == 0) {
			res = 1;
		}
		if(*pre_cmd == 'u')
			res = -res;
		res += currentID;
	}
	else if (strcmp(pre_cmd, "loc") == 0) {
		ptr = s + offset;
	}
	else if (strcmp(pre_cmd, "sav") == 0) {
		if (*(s + offset) != '\0') {
			cmdFragmentCopy(name, s + offset + 1, MAX_NAME);
			printf("cmdDealer::the exp name is: %s\n", name);
			mexps = getCurrentBufferExpressionSets(expb);
			nameExpressionSet(mexps, name);
			if (cmd_color == CMD_TRUE)
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), CMD_RESPONSE_COLOR);
			printf("cmdDealer::Saved as \"%s\" at %s%s\n", mexps->name, mdirBuffer, savDir);
			if (cmd_color == CMD_TRUE)
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
			savExpressionSet(getCurrentBufferExpressionSets(expb), SAVE_AUTO_NAME, IOMODE_A);
		}
		else
			throwError("After input 'sav', please input the name of expression, without enter\n", GREY);
	}

	else if (strcmp(pre_cmd, "shw") == 0) {
		if (*(s + offset) != '\0') {
			cmdFragmentCopy(cmd_speci, s + offset + 1, CMD_TEMP_MAX_BUFFER);
			if (strcmp(cmd_speci, "set") == 0) {
				printfSettings(mdirBuffer);
			}
			if (strcmp(cmd_speci, "rpn") == 0) {
				printRPN(frag, bufferHead);
			}
			if (strcmp(cmd_speci, "buf") == 0) {
				printfAllBuffer(expb);
			}
		}
	}
	else if (strcmp(pre_cmd, "ttr") == 0) {
		firstTimeWelcome(opStack, frag, expb);
	}
	else if (strcmp(pre_cmd, "clr") == 0) {
		cmdFragmentCopy(name, s + offset + 1, MAX_NAME);
		if (strcmp(cmd_speci, "all") == 0) {
			if (remove(bufferDir) == -1) throwError("cmdDealer::Fails to delete buffer file", GREY);
			if (remove(bufferIDDir) == -1) throwError("cmdDealer::Fails to delete buffer index file", GREY);
			if (remove(savNameDir)) throwError("cmdDealer::Fails to delete save index file", GREY);
			if (remove(savDir) == -1) throwError("cmdDealer::Fails to delete save file", GREY);
			expressionBufferIDReset(expb);
			throwError("cmdDealer::ID are changed, and the current ID is ", GREY);
			printf("%d\n", globe_ID - 1);
		}
		else if (strcmp(cmd_speci, "buf") == 0) {
			if (remove(bufferDir) == -1) throwError("cmdDealer::Fails to delete buffer file", GREY);
			if (remove(bufferIDDir) == -1) throwError("cmdDealer::Fails to delete buffer index file", GREY);
			expressionBufferIDReset(expb);
			throwError("cmdDealer::ID are changed, and the current ID is ", GREY);
			printf("%d\n", globe_ID - 1);
		}
		else if (strcmp(cmd_speci, "sav") == 0) {
			if (remove(savNameDir)) throwError("cmdDealer::Fails to delete save index file", GREY);
			if (remove(savDir) == -1) throwError("cmdDealer::Fails to delete save file", GREY);
		}
	}
	else if (strcmp(pre_cmd, "exit") == 0) {
		exit(0);
	}
	return res;
}

int initCommand() {
	cmd_color = CMD_TRUE;
	cmd_autoCorrect = CMD_TRUE;
	cmd_autoParenthese = CMD_TRUE;
	cmd_maxErrorLog = 4;
	cmd_TimeLimit = CMD_DEFAULT_TIME_LIMIT;
	return 0;
}

int cmdFragmentCopy(char *Destination, char *origin, int maxLength) {
	int i;
	for (i = 0; i < maxLength; i++) {
		*Destination = *origin;
		if (*origin == ' ' || *origin == '\0' || *origin == '\n') {
			*Destination = '\0';
			return i;
			break;
		}
		Destination++;
		origin++;
	}
	*--Destination = '\0';
	return i - 1;
}

void printHelp() {
	int i;
	for (i = 0; i < CMD_HELP_KINDS; i++) {
		
		printf("%s", cmd_list[i]);
		if (cmd_color == CMD_TRUE)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREY);
		printf("\t\t%s\n", cmd_explain[i]);
		if (cmd_color == CMD_TRUE)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	}
}
void printfSettings(char *currentDir) 
{
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREY);
	printf("printfSettings::cmd_autoCorrect = %d\tcmd_autoParenthese = %d\t\n"
		"\t\tcmd_color = %d\tcmd_maxErrorLog = %d\t\n", cmd_autoCorrect, cmd_autoParenthese, cmd_color, cmd_maxErrorLog);
	printf("current buffer directory: %s%s\n", currentDir, bufferDir);
	printf("current save directory: %s%s\n", currentDir, savDir);
}
void throwError(const char*errorLog, int color) {
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	printf(errorLog);
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
}



int varValueControl (ExpressionSets *exps, ExpresionBuffer *expb, char *s)
{
	ExpressionSets *myExps;
	int var;
	expressionSetsVarSync(exps);
	while (*s != '\n') {
		if ((var = isVarInTable(*s)) != -1) {
			//s only move to the next character of '='
			//which is required by cmdGetNum
			s++;
			if (*s != '=') {
				throwError("varValueControl::Unkown character: ", GREY);
				printf("%c\n", *s);
				return -1;
			}
			varValueConfig[var] = true;
			varValue[var] = cmdGetNum<PolyType>(s);
			//myExps->varTable[var] = -1;
			if (*s == '\n' || *s == '\0') {
				break;
			}
			//s should be pointing at ','
			s++;
			//here s has already moved to next var
		}
		else {
			throwError("varValueControl::Undefined variable\n", GREY);
			return -1;
		}
	}
	/*
	i = var = 0;
	for (i = 0; i < MAXVAR; i++) {
		if (myExps->varTable[i] != -1) {
			myExps->varTable[var++] = myExps->varTable[i];
		}
	}*/
	myExps = expressionSetsDuplicate(exps);
	//printExpression(*(myExps->base));
	caluclateExpression(myExps);
	pushExpressionBuffer(expb, myExps, true);
	printBufferCurrentOffset(expb);
	return 0;
}

//poorly designed cmd sysytem, ding it!
void cmdSync(int cmd[MAX_CMD])
{
	int flag = 0;
	if (cmd_autoCorrect != cmd[0]) {
		cmd_autoCorrect = cmd[0];
		flag = 1;
	}
}

void cmdCpy(double cmd[MAX_CMD])
{
	cmd[0] = cmd_autoCorrect;
	cmd[1] = cmd_autoParenthese;
	cmd[2] = cmd_autoSave;
	cmd[3] = cmd_color;
	cmd[4] = cmd_errorOccurance;
	cmd[5] = cmd_ErrorTimeGap;
	cmd[6] = cmd_maxErrorLog;
}

/*
float myLikeSigmod(float x) {
	float a = x;
	if (a < 0) {
		a = -a;
	}
	return (x / (1 + a)) / 2;
}

float myNormalDistribution(float x) {
	float sigma = sqrt(5);
	//float mu = 0;
	//return  (1.0 / (sigma * sqrt(2 * M_PI)) ) * exp(-(x - mu)*(x - mu) / (2.0 * sigma * sigma));
	return  (1.0 / (sigma * sqrt(2 * M_PI))) * exp(-x*x / (2.0 * sigma * sigma));
}
*/

void cmdDraw(PolyVarType x, PolyVarType y, PolyType step, PolyType range, PolyType offset_x, PolyType offset_y, ExpressionSets *exps)
{
	int i = 2 * int(range / step);
	int count = i;
	int j = i;
	PolyType pos_x, pos_y;
	double total_1, total_2;
	float a[6];
	FILE *fp;
	clearVarValue();
	pos_x = offset_x + range;
	pos_y = offset_y + range;

	fopen_s(&fp, "./res/draw.data", "wb");
	
	if (fp == NULL) {
		if (cmd_color == CMD_TRUE)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREY);
		printf("cmdDraw::Fails to open file\n");
		if (cmd_color == CMD_TRUE)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
		return;
	}
	// i
	//pos color
	//average height
	fwrite(&count, sizeof(int), 1, fp);
	a[3] = a[4] = a[5] = 0;
	total_2 = 0;
	for (; i > 0; i--) {
		a[0] = float(pos_x);
		total_1 = 0;
		pos_y = offset_y + range;
		for (; j > 0; j--) {
			a[1] = float(pos_y);
			varValue[x] = pos_x;
			varValue[y] = pos_y;
			a[2] = float(caluclateExpressionResult(exps));
			fwrite(a, sizeof(float), 6, fp);
			pos_y -= step;
			total_1 += a[2];
		}
		pos_x -= step;
		total_2 += total_1 / count;
		//a[3] = myLikeSigmod(a[2]) + 1;  //red
		//a[4] = myNormalDistribution(a[2]) * 2;
		//a[5] = -myLikeSigmod(a[2]) + 1
	}
	total_2 /= count;
	a[0] = float(total_2);
	fwrite(a, sizeof(float), 1, fp);
	if (fclose(fp)) {
		throwError("cmdDraw::Can't close file", GREY);
	}
}
