#include "IOControl.h"
#include "PolishReverse.h"
#include <windows.h>
#include  <io.h>
#include "command.h"

static FILE *globe_fp;
bool isFundamentalOperator(char c);
char bufferDir[20] = "./src/buf.data";
char bufferIDDir[20] = "./src/buf.idx";
char savDir[20] = "./src/sav.data";
char savNameDir[20] = "./src/sav.idx";

char* fgetExpression(char *buffer, const char *address) 
{
	static char exAddress[MAX_ADDRESS];
	static long offset;
	static char name[MAX_NAME];
	char temp_c;
	FILE *fp;
	//have the same value
	if (strcmp(exAddress, address) != 0) {
		offset = 0;
	}
	fopen_s(&fp, address, "r");
	if (fp == NULL) {
		if (cmd_color == CMD_TRUE)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREY);
		printf("freadExpression::Fails to open file\n"
			"Address: %s, exAdress, %s,  offset: %ld\n", address, exAddress,  offset);
		if (cmd_color == CMD_TRUE)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
	}
	fseek(fp, offset, SEEK_SET);
	while ((temp_c = fgetc(fp)) != '#' && temp_c != EOF) 
		;
	if (feof(fp)) {
		if (fclose(fp)) {
			throwError("freadExpression::Can't close file at ", GREY);
			printf("%s\n", address);
		}
		return NULL;
	}
	else {
		
		if ((temp_c = fgetc(fp)) == '\n') {
			name[0] = '\0';
		}
		else
		{
			name[0] = temp_c;
			fgets(name + 1, MAX_NAME - 1, fp);
		}
		
		while ((temp_c = fgetc(fp)) != '$' && temp_c != EOF)
			;
		fgets(buffer, MAX_BUFFER, fp);

		if (fclose(fp)) {
			throwError("freadExpression::Can't close file at ", GREY);
			printf("%s\n", address);
		}
		return name;
	}
	return NULL;
}
ExpressionSets *freadExpression(int ID, const char *FileAddress) {
	FILE *fp;
	ExpressionSets *exps = NULL;
	fopen_s(&fp, FileAddress, "rb");
	if (fp == NULL) {
		throwError("freadExpression::Fails to open file at ", GREY);
		puts(FileAddress);
	}
	fread(exps, expressionSetsSize, 1, fp);
	while (exps != NULL && exps->ID != ID && !feof(fp)) {
		fread(exps, expressionSetsSize, 1, fp);
	}
	if (feof(fp)) {
		throwError("freadExpression::Can't find expression whose id is", GREY);
		printf("%d\n", ID);
		exps = NULL;
	}
	if (fclose(fp)) {
		throwError("freadExpression::Can't close file at ", GREY);
		printf("%s\n", FileAddress);
	}
	return exps;
}

ExpressionSets *freadExpression_name(const char *exp_name, const char *FileAddress) {
	FILE *fp;
	ExpressionSets *exps = NULL;
	fopen_s(&fp, FileAddress, "rb");
	if (fp == NULL) {
		throwError("freadExpression_name::Fails to open file at ", GREY);
		puts(FileAddress);
	}
	fread(exps, expressionSetsSize, 1, fp);
	while (strcmp(exp_name, exps->name) != 0 && !feof(fp)) {
		fread(exps, expressionSetsSize, 1, fp);
	}
	if (strcmp(exp_name, exps->name) != 0)
	{
		throwError("freadExpression_name::Can't find expression whose name is", GREY);
		printf("%s\n", exp_name);
		exps = NULL;
	}
	if (fclose(fp)) {
		throwError("freadExpression_name::Can't close file at ", GREY);
		puts(FileAddress);
	}
	return exps;
}

void fwriteExpression_name(ExpressionSets *exps, const char *FileAddress) {
	FILE *fp;
	ExpressionSets *exps1 = (ExpressionSets *)malloc(expressionSetsSize);
	fopen_s(&fp, FileAddress, "rb+");
	if (fp == NULL) {
		fopen_s(&fp, FileAddress, "wb");
		if (fclose(fp)) {
			throwError("fwriteExpression::Can't close file at ", GREY);
			puts(FileAddress);
		}
		fopen_s(&fp, FileAddress, "rb+");
		if (fp == NULL) {
			throwError("fwriteExpression::Fails to open file at ", GREY);
			puts(FileAddress);
		}
	}
	fread(exps1, expressionSetsSize, 1, fp);
	while (!feof(fp) && strcmp(exps1->name, exps->name) != 0) {
		fread(exps1, expressionSetsSize, 1, fp);
	}
	if (strcmp(exps1->name, exps->name) == 0) {
		throwError("fwriteExpression::Expression whose name duplicated\n", GREY);
		printf("Input 0 to overwrite it. Input others to abort saving.\n");
		if (getchar() != '0') {
			fflush(stdin);
			if (fclose(fp)) {
				throwError("fwriteExpression::Can't close file at ", GREY);
				puts(FileAddress);
			}
			freeExpressionSets(exps1);
			return;
		}
		else {
			fwrite(exps, expressionSetsSize, 1, fp);
			if (fclose(fp)) {
				throwError("fwriteExpression::Can't close file at ", GREY);
				puts(FileAddress);
			}
			freeExpressionSets(exps1);
			return;
		}
		fflush(stdin);
	}
	fseek(fp, 0L, SEEK_END);
	fwrite(exps, expressionSetsSize, 1, fp);
	if (fclose(fp)) {
		throwError("fwriteExpression::Can't close file at ", GREY);
		puts(FileAddress);
	}
	freeExpressionSets(exps1);
	return;
}

int fwriteExpression_ID(ExpressionSets *exps,const char *FileAddress) {
	FILE *fp;
	ExpressionSets *exps1 = (ExpressionSets *)malloc(expressionSetsSize);
	fopen_s(&fp, FileAddress, "rb+");
	if (fp == NULL) {
		fopen_s(&fp, FileAddress, "wb");
		if (fclose(fp)) {
			throwError("fwriteExpression::Can't close file at ", GREY);
			puts(FileAddress);
		}
		fopen_s(&fp, FileAddress, "rb+");
		if (fp == NULL) {
			throwError("fwriteExpression::Fails to open file at ", GREY);
			puts(FileAddress);
		}
	}
	fread(exps1, expressionSetsSize, 1, fp);
	while (!feof(fp) && exps1->ID != exps->ID) {
		fread(exps, expressionSetsSize, 1, fp);
	}
	if (exps1->ID != exps->ID) {
		fwrite(exps, expressionSetsSize, 1, fp);
		if (fclose(fp)) {
			throwError("fwriteExpression::Can't close file at ", GREY);
			puts(FileAddress);
		}
		freeExpressionSets(exps1);
		return -1;
	}
	fseek(fp, 0L, SEEK_END);
	fwrite(exps, expressionSetsSize, 1, fp);
	if (fclose(fp)) {
		throwError("fwriteExpression::Can't close file at ", GREY);
		puts(FileAddress);
	}
	freeExpressionSets(exps1);
	return 0;
}

long fgetIDOffset(FILE *fp_idx, int ID)
{
	long myOffset;
	int myID;
	while (!feof(fp_idx)) {
		fread(&myID, sizeof(int), 1, fp_idx);
		fread(&myOffset, sizeof(long), 1, fp_idx);
		if (ID == myID) {
			return myOffset;
		}
	}
	return -1;
}

long fgetNameOffset(FILE *fp_idx, const char *name)
{
	long myOffset;
	char nameBuf[MAX_NAME];
	while (!feof(fp_idx)) {
		fread(&nameBuf, sizeof(char), MAX_NAME, fp_idx);
		fread(&myOffset, sizeof(long), 1, fp_idx);
		if (strcmp(nameBuf, name) == 0)
		{
			return myOffset;
		}
	}
	return -1;
}

int myOpenFile_write(char *FileName, FILE *&fp, int IOMode)
{
	if (_access(FileName, 0) == -1) {
		fopen_s(&fp, FileName, "wb+");
		fwrite(&IOMode, sizeof(int), 1, fp);
		fseek(fp, 0L, SEEK_SET);
	}
	else
		fopen_s(&fp, FileName, "rb+");
	if (fp == NULL) {
		throwError("savExpressionSet::Fails to open file at ", GREY);
		puts(FileName);
		return -1;
	}
	return 0;
}

//it will check with I/O Mode
int myOpenFile_read(char *FileName, FILE *&fp, int IOMode) 
{
	int myIOMode;
	fopen_s(&fp, FileName, "rb");
	if (fp == NULL) {
		throwError("freadExpressionSets::Fails to open file at ", GREY);
		puts(FileName);
		return -1;
	}
	fread(&myIOMode, sizeof(int), 1, fp);
	if (myIOMode != IOMode)
	{
		throwError("freadExpressionSets::Incompatible I/O Mode\n", GREY);
		fclose(fp);
		return -1;
	}
}

int savExpressionSet(ExpressionSets *exps, int SaveMode, int IOMode, char *FileName, char *File_idx_Name)
{
	FILE *fp = NULL, *fp_idx = NULL;
	char c;
	long offset;
	int a, b;
	if (SaveMode != 0) {
		if (SaveMode == SAVE_AUTO_ID)
		{
			if (myOpenFile_write(bufferDir, fp, IOMode) == -1) return -1;
			if (myOpenFile_write(bufferIDDir, fp_idx, IOMode) == -1) return -1;
			
			if ((c = *(exps->name)) != '\0') {
				*(exps->name) = '\0';
			}
			fwriteExpressionSets(fp, fp_idx, exps, offset, IOMode);
			*(exps->name) = c;
		}
		else if (SaveMode == SAVE_AUTO_NAME)
		{
			if (myOpenFile_write(savDir, fp, IOMode) == -1) return -1;
			if (myOpenFile_write(savNameDir, fp_idx, IOMode) == -1) return -1;
			
			fwriteExpressionSets(fp, fp_idx, exps, offset, IOMode);
		}
	}
	else
	{
		;
	}
	a = fclose(fp);
	b = fclose(fp_idx);
	return (a|b);
}
int fwriteExpressionSets(FILE *fp, FILE *fp_idx, ExpressionSets *exps, long &offset, int IOMode)
{
	int fileMode;
	char name[MAX_NAME];
	long myOffset;
	int ID;

	fread(&fileMode, sizeof(int), 1, fp_idx);
	if (fileMode != IOMode) {
		throwError("fwriteExpressionSets::Incompatible I/O Mode for index file\n", GREY);
		return -1;
	}
	fread(&fileMode, sizeof(int), 1, fp);
	if (fileMode != IOMode) {
		throwError("fwriteExpressionSets::Incompatible I/O Mode\n", GREY);
		return -1;
	}

	if (*(exps->name) == '\0') {
		while (!feof(fp_idx)) {
			fread(&ID, sizeof(int), 1, fp_idx);
			fread(&myOffset, sizeof(long), 1, fp_idx);
			if (ID == exps->ID) {
				throwError("fwriteExpressionSets::Duplicated ID, fails to write in expression\n", GREY);
				offset = myOffset;
				return -1;
			}
		}
		fseek(fp, 0L, SEEK_END);
		fseek(fp_idx, 0L, SEEK_END);
		myOffset = ftell(fp);
		fwrite(&(exps->ID), sizeof(int), 1, fp_idx);
		fwrite(&myOffset, sizeof(long), 1, fp_idx);
		fwriteAnExpression(fp, exps);
	}
	else {
		while (!feof(fp_idx)) {
			fread(&name, sizeof(char), MAX_NAME, fp_idx);
			fread(&myOffset, sizeof(long), 1, fp_idx);
			if (strcmp(name, exps->name) == 0) {
				throwError("fwriteExpressionSets::Duplicated Name, fails to write in expression\n", GREY);
				offset = myOffset;
				return -1;
			}
		}
		fseek(fp, 0L, SEEK_END);
		myOffset = ftell(fp);
		fwrite(&(exps->name), sizeof(char), MAX_NAME, fp_idx);
		fwrite(&myOffset, sizeof(long), 1, fp_idx);
		fwriteAnExpression(fp, exps);
	}
	return 0;
}

void fwriteAnExpression(FILE *fp, ExpressionSets *exps) 
{
	ExpressionSetsElem *moveable_base;
	Expressions *myexp;
	Poly *mypoly;
	int count_exp, count_var, count;
	int cmd[MAX_CMD];
	char name[MAX_NAME];
	count = 0;
	moveable_base = exps->base;
	while (moveable_base < exps->top) {
		count++;
		moveable_base++;
	}
	moveable_base = exps->base;

	fwrite(&cmd, sizeof(int), MAX_CMD, fp);
	fwrite(&(exps->ID), sizeof(int), 1, fp);
	fwrite(name, sizeof(char), MAX_NAME, fp);
	fwrite(exps->varTable, sizeof(char), MAXVAR, fp);
	fwrite(&count, sizeof(int), 1, fp);

	while(moveable_base < exps->top) {
		myexp = *moveable_base;
		count_exp = 0;
		while (myexp) {
			myexp = myexp->next;
			count_exp++;
		}
		fwrite(&count_exp, sizeof(int), 1, fp);
		myexp = *moveable_base;
		while(myexp) {
			mypoly = myexp->son;
			count_var = 0;
			while (mypoly) {
				mypoly = mypoly->son;
				count_var++;
			}
			fwrite(&count_var, sizeof(int), 1, fp);
			fwrite(&(myexp->coeff), sizeof(PolyType), 1, fp);

			mypoly = myexp->son;
			while (mypoly) {
				fwrite(&(mypoly->degree), sizeof(PolyType), 1, fp);
				fwrite(&(mypoly->var), sizeof(PolyVarType), 1, fp);
				mypoly = mypoly->son;
			}
			myexp = myexp->next;
		}
		moveable_base++;
	}
}



ExpressionSets *freadExpressionSets(ExpressionSets *&exps, int SaveMode, int IOMode, int ID, const char *name, const char *FileName, const char *File_idx_Name, bool hasID)
{
	long myOffset;
	//int myIOMode;
	FILE *fp = NULL, *fp_idx = NULL;
	if (SaveMode != 0) {
		if (SaveMode == SAVE_AUTO_ID)
		{
			if (myOpenFile_read(bufferDir, fp, IOMode) == -1) {
				exps = NULL;
				//throwError("freadExpressionSets::Wrong I/O Mode\n", GREY);
				return NULL;
			}
			if (myOpenFile_read(bufferIDDir, fp_idx, IOMode) == -1) {
				exps = NULL;
				//throwError("freadExpressionSets::Wrong index I/O Mode \n", GREY);
				return NULL;
			}
			
			myOffset = fgetIDOffset(fp_idx, ID);
			if (myOffset == -1) {
				throwError("freadExpressionSets::can't find such ID\n", GREY);
				fclose(fp);
				fclose(fp_idx);
				return NULL;
			}
			fseek(fp, myOffset, SEEK_SET);
			freadAnExpression(fp, exps, hasID);
		}
		else if (SaveMode == SAVE_AUTO_NAME)
		{
			if (myOpenFile_read(savDir, fp, IOMode) == -1) {
				exps = NULL;
				return NULL;
			}
			if (myOpenFile_read(savNameDir, fp_idx, IOMode) == -1) {
				exps = NULL;
				return NULL;
			}
			
			myOffset = fgetNameOffset(fp_idx, name);
			if (myOffset == -1) {
				throwError("freadExpressionSets::can't find such name: ", GREY);
				printf("%s\n", name);
				fclose(fp);
				fclose(fp_idx);
				return NULL;
			}
			fseek(fp, myOffset, SEEK_SET);
			freadAnExpression(fp, exps, hasID);
		}
	}
	fclose(fp);
	fclose(fp_idx);
	return exps;
}


void freadAnExpression(FILE *fp, ExpressionSets *&exps, bool hasID)
{
	//ExpressionSetsElem *moveable_base;
	Expressions *myexp, *myexpHead;
	Poly *mpoly;
	int ID_dump;
	int count_exp, count_var, count;
	int cmd[MAX_CMD];
	//char name[MAX_NAME];

	exps = initExpressiosSets(hasID, NULL);

	fread(&cmd, sizeof(int), MAX_CMD, fp);
	if (hasID == false) {
		fread(&(exps->ID), sizeof(int), 1, fp);
	}
	else {
		fread(&ID_dump, sizeof(int), 1, fp);
	}
	fread((exps->name), sizeof(char), MAX_NAME, fp);
	fread(exps->varTable, sizeof(char), MAXVAR, fp);
	fread(&count, sizeof(int), 1, fp);

	for (int i = 0; i < count; i++) {
		myexp = myexpHead = NULL;
		fread(&count_exp, sizeof(int), 1, fp);
		for (int j = 0; j < count_exp; j++) {
			if (myexpHead == NULL) {
				myexpHead = myexp = (Expressions *)malloc(ExpressionSize);
			}
			else {
				myexp->next = (Expressions *)malloc(ExpressionSize);
				myexp = myexp->next;
			}
			myexp->son = NULL;
			myexp->next = NULL;
			fread(&count_var, sizeof(int), 1, fp);
			fread(&(myexp->coeff), sizeof(PolyType), 1, fp);
			mpoly = NULL;
			for (int k = 0; k < count_var; k++) {
				if (mpoly == NULL) {
					myexp->son = mpoly = (Poly *)malloc(PolySize);
				}
				else {
					mpoly->son = (Poly *)malloc(PolySize);
					mpoly = mpoly->son;
				}
				fread(&(mpoly->degree), sizeof(PolyType), 1, fp);
				fread(&(mpoly->var), sizeof(PolyVarType), 1, fp);
				mpoly->son = NULL;
			}
		}
		pushExpressiosSets(exps, myexpHead);
	}
}















int stringLegalchecker(char*s) 
{
	int legal = 0;
	int errors = 0;
	char c;
	int i;
	char *head;
	HANDLE hwdl = GetStdHandle(STD_OUTPUT_HANDLE);
	head = s;

	c = *s;
	if (isOperator(c)) {
		if (cmd_color == CMD_TRUE)
			SetConsoleTextAttribute(hwdl, GREY);
		printf("stringLegalchecker::The first character: ");
		if (cmd_color == CMD_TRUE)
			SetConsoleTextAttribute(hwdl, RED);
		printf("%c ", c);
		throwError("is not a number\n", GREY);
		errors++;
		legal = -1;
		if (errors > cmd_maxErrorLog) {
			return -1;
		}
	}
	while (!isStringEnd(c)) {
		if (isNumber(c) || isOperator(c) || isVariable(c)) {
			if (c == '^' && !isStringEnd(*(s + 1)) ) {
				i = 1;
				//a^-abc
				if (*(s + 1) == '-') {
					if (!isStringEnd(*(s + 2)) && isVariable(*(s + 2))) {
						throwError("stringLegalchecker::Index has variable\n", GREY);
						errors++;
						legal = -1;
						if (errors > cmd_maxErrorLog) {
							return -1;
						}
					}
				}
				else if (isVariable(*(s + 1))) {
					throwError("stringLegalchecker::Index has variable\n", GREY);
					errors++;
					legal = -1;
					if (errors > cmd_maxErrorLog) {
						return -1;
					}
				}
			}
		}
		else if (c == ' ' && legal == 0) {
			legal = 1;
		}
		else {
			if (cmd_color == CMD_TRUE)
				SetConsoleTextAttribute(hwdl, GREY);
			printf("stringLegalchecker::unknow character ");
			if (cmd_color == CMD_TRUE)
				SetConsoleTextAttribute(hwdl, RED);
			printf("%c\n", c);
			if (cmd_color == CMD_TRUE)
				SetConsoleTextAttribute(hwdl, WHITE);
			errors++;
			legal = -1;
			if (errors > cmd_maxErrorLog) {
				return -1;
			}
		}
		if (isFundamentalOperator(*s) && isFundamentalOperator(*(s + 1)) &&
			!isStringEnd(*(s + 1)) && !isStringEnd(*(s + 2)) && !(*(s+1) == '-') && isNumber(*(s + 2))) {
			if (cmd_color == CMD_TRUE)
				SetConsoleTextAttribute(hwdl, GREY);
			printf("stringLegalchecker::Dupilicated binary operaters: ");
			if (cmd_color == CMD_TRUE)
				SetConsoleTextAttribute(hwdl, RED);
			printf("%c%c\n", *s, *(s+1));
			if (cmd_color == CMD_TRUE)
				SetConsoleTextAttribute(hwdl, WHITE);
			errors++;
			legal = -1;
			if (errors > cmd_maxErrorLog) {
				return -1;
			}
		}
		c = *++s;
	}
	
	while (isFundamentalOperator(*s) && s > head) {
		s--;
		throwError("stringLegalchecker::Binary operator at the end of expression\n", GREY);
	}
	return legal;
}

int parentheseCherker(char *s)
{
	int left = 0, right = 0;
	char c = *s;
	while (!isStringEnd(c)) {
		if (c == '('){
			left++;
		}
		else if(c == ')')
		{
			right++;
		}
		c = *++s;
	}
	return left - right;
}

// return new length
int compactArray(char *s, char c)
{
	int i, j;
	for (i = 0, j = 0; s[i]; i++) {
		if (s[i] != c) {
			s[j++] = s[i];
		}
	}
	s[j] = '\0';
	return j;
}

int trimSpaces(char* s)
{
	int i, j;
	for (i = 0, j = 0; s[i]; i++) {
		if (!isspace(s[i])) {
			s[j++] = s[i];
		}
	}
	s[j] = '\0';
	return j;
}

bool isFundamentalOperator(char c) {
	if (c == '+' || c == '-' || c == '*' || c == '^') {
		return true;
	}
	else {
		return false;
	}
}

void expressionAutoCorrector(char *s) {
	int i, j, k;
	char c;
	int flag = 0;
	for (i = 0, j = 0; c = s[i]; i++) {
		if (s[i] == '(' ) {
			if (s[i + 1] == ')') {
				i = i + 1;
				continue;
			}
			k = i + 1;
			if (s[k] == '^' || s[k] == '*' || s[k] == '+') {
				s[j++] = s[i];
				while (s[k] == '^' || s[k] == '*' || s[k] == '+') {
					k++;
				}
				i = k - 1;
			}
		}
		if (isNumber(c) || isOperator(c) || isVariable(c))
		{
			s[j++] = s[i];
		}
	}
	s[j] = '\0';
	if (j < i-1) {
		flag = 1;
	}
	//duplicate ops like a++--**b will be regarded as a*b
	for (i = 0, j = 0; s[j]; i++,j++) {
		k = 1;
		while (!isStringEnd(*(s + j + k)) && isFundamentalOperator(*(s+j)) && isFundamentalOperator(*(s + j+k))) {
			k++;
			flag = 1;
		}
		if (*(s + j + k -1) == '-' && isNumber(*(s + j + k)) && k>1) {
			s[i++] = s[j + k - 2];
		}
		j += k-1;
		s[i] = s[j];
	}
	s[i] = '\0';
	for (i = 0; s[i+1]; i++) {
		if (s[i] == '^' && isVariable(s[i + 1])) {
			flag = 1;
			s[i] = '*';
		}
	}
	while (isFundamentalOperator(s[i])) {
		s[i] = '\0';
		i--;
	}
	if (flag) {
		throwError("This Expression is automatically corrected into:\n", GREY);
		printf("%s\n", s);
		throwError("You can turn off corrector if you don't want it.\n", GREY);
	}
		
}

int parentheseAutoAdder(char *s, int num, int maxBuffer) 
{
	int offset, count, i;
	bool isOp;
	int flag = 0;
	char *end = s;
	offset = 0;
	while (*end != '\0') {
		end++;
	}
	count = end - s + 1;
	if (*s == '+' || *s == '*' || *s == '^') {
		isOp = true;
	}
	else
		isOp = false;

	if (isOp || num < 0) {
		if (isOp) {
			count++;
			offset++;
			
		}
		if (num < 0) {
			count -= num;
			offset -= num;
		}
		if (count > maxBuffer) {
			throwError("parentheseAutoAdder::Exceeds max buffer\n", GREY);
			return -1;
		}
		else {
			for (i = count; i >= offset; i--) {
				s[i] = s[i - 1];
			}
			if (isOp) {
				s[i--] = '1';
			}
			for (; i >= 0; i--) {
				s[i] = '(';
			}
		}
		flag = 1;
	}
	if (num > 0) {
		if (count + num > maxBuffer) {
			throwError("parentheseAutoAdder::Exceeds max buffer\n", GREY);
			return -1;
		}
		s[count + num - 1] = '\0';
		for (i = num; i > 0; i--) {
			s[count + i - 2] = ')';
		}
		flag = 1;
	}
	return flag;
}

//abc^2+abc*2(abc+a7x)^54abc