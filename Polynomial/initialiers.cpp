#include "initializer.h"
#include "command.h"
#include "PolishReverse.h"
#include "IOControl.h"

#include "errorDealer.h"
#include <Windows.h>
#include <io.h>
#include <direct.h>

int iniHello = 0;
char iniName[30] = "./src/preference.ini";




void initIOControl()
{
	FILE *fp;
	char dirName[10] = "./src";
	
	int int_dump;
	int mID;
	long offset_dump;
	if (_access(dirName, 0) == -1) {
		_mkdir(dirName);
		if (_access(dirName, 0) == -1) {
			std::cerr << "initIOControl::Create directory failed, may fails to record" << std::endl;
		}
	}
	if (_access(iniName, 0) == -1) {
		fopen_s(&fp, iniName, "wb+");
	}
	else {
		fopen_s(&fp, iniName, "rb+");
		iniHello = 1;
	}
	if (fp == NULL) {
		std::cerr << "initIOControl::Can't open" << iniName << std::endl;
	}
	else {
		;
	}
	fclose(fp);

	globe_ID = 1;
	if (_access(bufferIDDir, 0) == -1) {
		;
	}
	else {
		fopen_s(&fp, bufferIDDir, "rb");
		if (fp == NULL) {
			std::cerr << "initIOControl::Can't open" << bufferIDDir << std::endl;
		}
		fread(&int_dump, sizeof(int), 1, fp);
		while (!feof(fp)) {
			fread(&mID, sizeof(int), 1, fp);
			fread(&offset_dump, sizeof(long), 1, fp);
			if (mID >= globe_ID) {
				globe_ID = mID + 1;
			}
		}
	}
	fclose(fp);
}

void preferenceUpadate(int UpadateMode, int *Para1, char *Para2)
{
	FILE *fp;
	fopen_s(&fp, iniName, "rb+");
	if (fp == NULL) {
		std::cerr << "preferenceUpadate::Can't open" << iniName << std::endl;
	}
	switch (UpadateMode)
	{
	case PREFERENCE_UPDATE_ID: {
		fwrite(Para1, sizeof(int), 1, fp);
		break;
	}
	default:
		break;
	}
	fclose(fp);
}
void firstTimeWelcome(OperatorStack *opStack, FragmentStack *fragStack, ExpresionBuffer *expBuffer)
{
	char buffer[MAX_BUFFER];
	char temp_buffer[MAX_BUFFER];
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), LIGHT_WHITE);
	cmd_color = CMD_FALSE;
	printf("Welcome! Here you can input expressions in a habitual way£¨e.g. 3x*(6y^2+3z+x*z))\n");
	printf("After your input, you can assign values to certain variables and get the results\n");
	printf("Here is an example for you:\n"); 
	printf("Step1:enter an expression like \"3xyz+4y^2-x*(yz+abc)\"\n"); fflush(stdin);  
	printf("Step2:press enter to get the result. You can see the result\'2xyz-xabc+4y^2\'\n"); fgets(buffer, MAX_BUFFER, stdin);
	if (*buffer == '\n') strcpy_s(buffer, "3xyz+4y^2-x*(yz+abc)"); 
	compactArray(buffer, ' ');
	if (analysisString(buffer) == 0) {
		;
	}
	else {
		printf("Something wrong with your input, we will use default expression for demonstration\n");
		strcpy_s(buffer, "3xyz+4y^2-x*(yz+abc)");
	}
	clearOperatorStack(opStack);
	clearFragmentStack(fragStack);
	clearVarValue();
	clearVarTable();
	RPN(buffer, opStack, fragStack);
	cal(fragStack, expBuffer);
	printf("\n");
	printf("Step3:enter\'x=1,y=2,z=3\'to assign values to variables, press enter to get results, you will see\'-abc+28\'\n");
	fgets(buffer, MAX_BUFFER, stdin);
	if (*buffer == '\n') strcpy_s(buffer, "x=1,y=2,z=3");
	else compactArray(buffer, ' ');
	if (isVariable(buffer[0]) && buffer[1] == '=') {
		;
	}
	else
	{
		printf("Something wrong with your input, we will use default expression for demonstration\n");
		strcpy_s(buffer, "3xyz+4y^2-x*(yz+abc)");
		clearOperatorStack(opStack);
		clearFragmentStack(fragStack);
		clearVarValue();
		clearVarTable();
		RPN(buffer, opStack, fragStack);
		cal(fragStack, expBuffer);
		strcpy_s(buffer, "x=1,y=2,z=3");
	}
	varValueControl(getCurrentBufferExpressionSets(expBuffer), expBuffer, buffer);
	printf("\n");
	
	printf("Step4:You can use reference to save you from the pain of inputing expression\n"); fflush(stdin);
	printf("      \"#123\" will be replaced by the expression whose ID is 123. Input \"(#1+#2+a)*2\" \n");
	fgets(buffer, MAX_BUFFER, stdin);
	if (*buffer == '\n') strcpy_s(buffer, "(#1+#2+a)*2");
	compactArray(buffer, ' ');
	if (analysisString(buffer) == 0) {
		;
	}
	else {
		printf("Something wrong with your input, we will use default expression for demonstration\n");
		strcpy_s(buffer, "(#1+#2+a)*2");
	}
	clearOperatorStack(opStack);
	clearFragmentStack(fragStack);
	clearVarValue();
	clearVarTable();
	RPN(buffer, opStack, fragStack);
	cal(fragStack, expBuffer);
	printf("\n");

	fflush(stdin);
	printf("Step5:Another way is to save the expression with a name (letter only), press Enter or input \"\\sav EXAMPLE\"\n");
	strcpy_s(temp_buffer, "\\sav EXAMPLE");
	cmdDealer(temp_buffer, expBuffer, fragStack, opStack, buffer);
	printf("\n");

	fflush(stdin);
	printf("Step5:Now you can refer to EXAMPLE by number sign: \"#EXAMPLE\", or put it in an expresion to be calculated\n");
	fgets(buffer, MAX_BUFFER, stdin);
	if (*buffer == '\n') strcpy_s(buffer, "#EXAMPLE");
	compactArray(buffer, ' ');
	if (analysisString(buffer) == 0) {
		;
	}
	else {
		printf("Something wrong with your input, we will use default expression for demonstration\n");
		strcpy_s(buffer, "#EXAMPLE");
	}
	clearOperatorStack(opStack);
	clearFragmentStack(fragStack);
	clearVarValue();
	clearVarTable();
	RPN(buffer, opStack, fragStack);
	cal(fragStack, expBuffer);
	printf("\n");

	printf("To get more information, please read instructions in our report\n");
	printf("Color filtered will be turned off. If it causes any inconvenience, input \"\\color off\"\n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	cmd_color = CMD_TRUE;
	//printf("Please enter the expression and press enter to indicate the end and get results\n");
	printf("Now play with the program or find out more settings by inputting \"\\help\"\n");
}

void initialWelcome()
{
	HANDLE hwdl = GetStdHandle(STD_OUTPUT_HANDLE);
	printf("You can input any exprssion.\n");
	printf("Operators this program supports: ");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(hwdl, LIGHT_BLUE);
	printf("-");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(hwdl, WHITE);
	printf(", ");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(hwdl, LIGHT_BLUE);
	printf("+");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(hwdl, WHITE);
	printf(", ");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(hwdl, LIGHT_BLUE);
	printf("*");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(hwdl, WHITE);
	printf(", ");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(hwdl, LIGHT_BLUE);
	printf("^");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(hwdl, WHITE);
	printf(".\n");
	printf("If you have input anything wrong, the program will try to correct it\n");
	printf("Type in \'\\help\' to get setting list. Type in \'\\ttr\' to get tutorial\n");
}





/*set font code from here: https://social.msdn.microsoft.com/Forums/windowsdesktop/en-US/c87c4bc1-b25c-4031-a062-32edbd16a75f/console-application-font-size-type-etc?forum=windowsgeneraldevelopmentissues */
/*
void setMyCurrentConsolefont()
{
	HANDLE hOut;
	hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_FONT_INFOEX lpConsoleCurrentFontEx;
	lpConsoleCurrentFontEx.cbSize = sizeof(CONSOLE_FONT_INFOEX);
	lpConsoleCurrentFontEx.dwFontSize.X = 8;
	lpConsoleCurrentFontEx.dwFontSize.Y = 15;
	lpConsoleCurrentFontEx.FontWeight = 100;
	lpConsoleCurrentFontEx.nFont = 1;
	lpConsoleCurrentFontEx.FontFamily = FF_DECORATIVE;
	lstrcpyW(lpConsoleCurrentFontEx.FaceName, L"Ariel");

	int Status;
	Status = SetCurrentConsoleFontEx(hOut, false, &lpConsoleCurrentFontEx);

	if (Status == 0) //skipped
	{
		Status = GetLastError();
		std::cout << "SetConsoleTitle() failed! Reason : " << Status << std::endl;
		exit(Status);
	}
}*/