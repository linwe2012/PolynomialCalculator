#ifndef _IOCONTROL_H_
#define _IOCONTROL_H_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ExpressionSet.h"


#define MAX_ADDRESS 100
#define MAX_BUFFER 1000
#define DEFAULT_SAV_FILE_LOCATION "./res/sav.exp"
#define DEFAULT_SAV_FILE_NAME "sav.exp"
#define DEFALT_TEMP_FILE_LOCATION "./res/temp.exp"
#define DEFALT_BUFFER_FILE_LOCATION "./res/temp.exp"

extern char bufferDir[20];
extern char bufferIDDir[20];
extern char savDir[20];
extern char savNameDir[20];

#define SAVE_AUTO_ID 1
#define SAVE_AUTO_NAME 2
#define IOMODE_A 1
/**
* @brief: 
* @param [in] exp1: 
* @param [in] exp2:
* @exception
* @note
*/
char* fgetExpression(char *buffer, char *address);
/**
* @brief: check if it is a legal expression
* @param [in] s: a pointer to the string to be check;
* @exception
* @return 0:is Totally Legal, which contains only variables, number and operater
		  1:is partly legal, which contains blanks and other legal stuff
* @note
*/
int stringLegalchecker(char*s);
int compactArray(char *s, char c);
int trimSpaces(char* s);
void expressionAutoCorrector(char *s);
int parentheseCherker(char *s);
int parentheseAutoAdder(char *s, int num, int maxBuffer);


inline bool isFundamentalOperator(char c);
ExpressionSets *freadExpression(int ID, const char *FileAddress);
void throwError(const char*errorLog, int color);


int savExpressionSet(ExpressionSets *exps, int SaveMode, int IOMode, char *FileName = NULL, char *File_idx_Name = NULL);
ExpressionSets *freadExpressionSets(ExpressionSets *&exps,
	int SaveMode,
	int IOMode,
	int ID,
	const char *name,
	const char *FileName = NULL,
	const char *File_idx_Name = NULL,
	bool hasID = true);

//don't use function down there
/**
* @brief: write in Expression According to name, usually saved by user
* @param [in] exps: a pointer to expression set to be written;
* @param [in] FileAddress: the loaction of the file;
* @exception
* @return 
* @note: if duplicated name is detected, it will warn user to overwrite it or not;
*/
void fwriteExpression_name(ExpressionSets *exps, const char *FileAddress);
/**
* @brief: write in Expression According to ID, usually used to save buffer
* @param [in] exps: a pointer to expression set to be written;
* @param [in] FileAddress: the loaction of the file;
* @exception
* @return 0:fine  -1:duplicted ID
* @note: if duplicated ID is detected, it will automatically overwrite, with a return Value -1;
*/
int fwriteExpression_ID(ExpressionSets *exps, const char *FileAddress);

/**
* @brief: write in expression without id, name or count
* @param [in] exps: a pointer to expression set to be written;
* @exception
* @note: it should not be used without calling other function
*/
void fwriteAnExpression(FILE *fp, ExpressionSets *exps);
void freadAnExpression(FILE *fp, ExpressionSets *&exps, bool hasID);
int fwriteExpressionSets(FILE *fp, FILE *fp_idx, ExpressionSets *exps, long &offset, int IOMode);



#endif // !_IOCONTROL_H_

