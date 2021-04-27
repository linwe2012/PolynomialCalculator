#include "ExpressionSet.h"
#include "command.h"
#include "IOControl.h"
#include "PolyCalculator.h"
#include "initializer.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define OPERATOR_COLOR WHITE
#define NUMBER_COLOR WHITE
#define VARIABLE_COLOR WHITE
#define INDEX_COLOR YELLOW



int expressionSetsSize = sizeof(ExpressionSets);
int expressionBufferSize = sizeof(ExpresionBuffer);
//note that globe_id always stores a future ID to be used
int globe_ID;

void initExpressionBuffer(ExpresionBuffer *expb)
{
	
	expb->base = (ExpresionBufferElem *)malloc(EXPRESSION_MAX_BUFFER * expressionSetsSize);
	if (expb == NULL) {
		throwError("initExpressionBuffer::fails to initialize expresion buffer. The program will end.\n", RED);
		getchar(); getchar(); getchar(); getchar();
		exit(0);
	}
	expb->top = expb->base;
	expb->maxSize = EXPRESSION_MAX_BUFFER;
	expb->currentID = 0;
	expb->offset = 0;
}

void pushExpressionBuffer(ExpresionBuffer *expb, ExpresionBufferElem exps, bool ifSave)
{
	int stackSize = expb->maxSize;
	if (ifSave && cmd_autoSave == CMD_TRUE) {
		savExpressionSet(exps, SAVE_AUTO_ID, IOMODE_A);
		preferenceUpadate(PREFERENCE_UPDATE_ID, &globe_ID);
	}
	if (expb->top - expb->base >= stackSize)
	{
		while (expb->top > expb->base) {
			expb->top--;
			freeExpressionSets(*(expb->top));
		}
	}
	expb->currentID = exps->ID;
	*(expb->top) = exps;
	expb->top++;
	expb->offset = expb->top - expb->base;
}

ExpressionSets *getCurrentBufferExpressionSets(ExpresionBuffer *expb)
{
	if (expb->base == expb->top)
		return NULL;
	return *(expb->base + expb->offset - 1);
}

void expressionBufferOffsetUpdate(ExpresionBuffer *expb, int ID)
{
	ExpresionBufferElem *moveable_top = (expb->top);
	while (moveable_top > expb->base) {
		moveable_top--;
		if ((*moveable_top)->ID == ID) {
			expb->currentID = ID;
			expb->offset = moveable_top - expb->base + 1;
		}
	}
}
//set the ID from zero, it will change globe ID
void expressionBufferIDReset(ExpresionBuffer *expb)
{
	ExpresionBufferElem *moveable_base = expb->base;
	int i = 1;
	while (moveable_base < expb->top) {
		if (expb->currentID == (*moveable_base)->ID) {
			expb->currentID = i;
		}
		(*moveable_base)->ID = i;
		i++;
		moveable_base++;
	}
	globe_ID = i;
}

ExpressionSets *initExpressiosSets(bool hasID, ExpressionSets *exps,  int initSize)
{
	if (exps == NULL) {
		exps = (ExpressionSets *)malloc(expressionSetsSize);
		if (exps == NULL) {
			throwError("initExpressiosSets::fails to initialize expression set. The program will end.\n", RED);
			getchar(); getchar(); getchar(); getchar();
			exit(0);
		}
	}
	exps->base = (ExpressionSetsElem *)malloc(initSize * ExpressionSize);
	if (exps->base == NULL) {
		throwError("initExpressiosSets::fails to initialize expresion set\'s base. The program will end.\n", RED);
		getchar(); getchar(); getchar(); getchar();
		exit(0);
	}
	exps->top = exps->base;
	exps->maxSize = initSize;
	if (hasID == true) {
		exps->ID = globe_ID;
		globe_ID++;
	}
	else {
		exps->ID = 0;
	}
	*(exps->name) = '\0';
	return exps;
}

void pushExpressiosSets(ExpressionSets *exps, ExpressionSetsElem exp)
{
	int stackSize = exps->maxSize;
	if (exps->top - exps->base >= stackSize)
	{
		exps->base = (ExpressionSetsElem *)realloc(exps->base, (stackSize + EXPRESSION_SETS_INCRETMENT) * ExpressionSize);
		if (exps == NULL) {
			throwError("pushExpressiosSets::fails to realloc. The program will end.\n", RED);
			getchar(); getchar(); getchar(); getchar();
			exit(0);
		}
		exps->top = exps->base + stackSize;
		exps->maxSize = stackSize + EXPRESSION_SETS_INCRETMENT;
	}

	*(exps->top) = exp;
	(exps->top)++;
}

ExpressionSetsElem popExpressiosSets(ExpressionSets *exps, ExpressionSetsElem *exp)
{
	if (exps->top == exps->base)
		return NULL;
	if (exp != NULL)
		*exp = *--(exps->top);
	else
		--(exps->top);
	return *(exps->top);
}

void freeExpressionSets(ExpressionSets *&exps)
{
	Expressions *exp;
	if (exps == NULL) 
		return;
	((exps)->top)--;
	while ((exps)->base <= (exps)->top)
	{
		exp = *((exps)->top);
		((exps)->top)--;
		freeExpression(exp);
	}
	free(exps);
}

int nameExpressionSet(ExpressionSets *exps, char *name)
{
	int i = 0;
	char *expsn = exps->name;
	char *head = name;
	int res = 0;
	while (!isStringEnd(*name)) {
		if(isLegalExpressionSetNameChar(*name))
			expsn[i++] = *name;
		name++;
	}
	if (head - name > i) {

	}
	expsn[i] = '\0';
	if (isStringEnd(*head))
	{
		for (i = 0; i < MAX_NAME - 1; i++)
			expsn[i] = getRandomLetter();
		expsn[i] = '\0';
		return -1;
	}
	return 0;
}

//it will stop at non-alphabet
void nameLegalizeCpy(char *destination, const char *source) {
	int i = 0;
	while (isLegalExpressionSetNameChar(*source)) {
		destination[i++] = *source;
		source++;
	}
	destination[i] = '\0';
}

bool isLegalExpressionSetNameChar(char c)
{
	return ((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A'));
}

char getRandomLetter()
{
	int num = rand() % (26 * 2 - 2);
	if (num < 26) {
		return num + 'a';
	}
	else {
		return num - 26 + 'A';
	}
}

void caluclateExpression(ExpressionSets *exps)
{
	Expressions *exp, *ptr;
	ExpressionSetsElem *moveable_base = exps->base;
	Poly *poly, *poly1;
	while (moveable_base < exps->top)
	{
		ptr = exp = *moveable_base;
		while (exp != NULL) {
			poly1 = exp->son;
			if (exp->coeff == 0) {
				//it only has a head
				if (ptr->next == NULL && ptr == *moveable_base) {
					freePolyList(ptr->son);
					break;
				}
				else {
					ptr->next = exp->next;
					freePolyList((exp->son));
					free(exp);
					exp = ptr->next;
				}
				continue;
			}

			while (poly1 && varValueConfig[poly1->var]) {
				exp->coeff *= PolyType(pow(varValue[poly1->var], poly1->degree));
				poly1 = poly1->son;
				free(exp->son);
				exp->son = poly1;
			}
			if (poly1 == NULL) {
				poly = NULL;
			}
			else {
				poly = poly1->son;
			}
			while (poly) {
				if (varValueConfig[poly->var]) {
					exp->coeff *= PolyType (pow(varValue[poly->var], poly->degree));
					poly1->son = poly->son;
					free(poly);
					poly = poly1->son;
				}
				if (poly == NULL) {
					break;
				}
				poly1 = poly;
				poly = poly->son;
			}
			if (exp == NULL) {
				break;
			}
			ptr = exp;
			exp = exp->next;
			//printExpression(*moveable_base);
		}
		/*
		//merge the same expressions
		ptr = exp = *moveable_base;
		ptr = ptr->next;
		while (ptr)
		{
			if (ptr && compare(ptr, exp) == 0) {
				while (ptr && compare(ptr, exp) == 0)
				{
					exp->coeff += ptr->coeff;
					exp->next = ptr->next;
					freePolyList(ptr->son);
					free(ptr);
					ptr = exp->next;
				}
			}
			else
			{
				exp = ptr;
				ptr = ptr->next;
			}
		}*/
		expressionSorter(*moveable_base);
		moveable_base++;
	}
}



PolyType caluclateExpressionResult(ExpressionSets *exps) {
	Expressions *exp;
	ExpressionSetsElem *moveable_base = exps->base;
	Poly *poly;
	PolyType res = 0, temp;

	//while (moveable_base < exps->top)
	//{
		exp = *moveable_base;
		while (exp != NULL) {
			poly = exp->son;
			temp = 1;
			while (poly) {
				temp *= PolyType(pow(varValue[poly->var], poly->degree));
				if (temp == 0)
					break;
				poly = poly->son;
			}
			res += temp;
			exp = exp->next;
		}
		//moveable_base++;

	//}
	return res;
}

void clearVarValue()
{
	for (int i = 0; i < MAXVAR; i++) {
		varValue[i] = 0;
		varValueConfig[i] = false;
	}
}
void clearVarTable()
{
	for (int i = 0; i < MAXVAR; i++) {
		varTable[i] = -1;
	}
}


void printBufferCurrentOffset(ExpresionBuffer *expb)
{
	printExpressionSet(*(expb->base + expb->offset - 1));
}

void printExpressionSet(ExpressionSets *exps)
{
	if (!exps || exps->base == exps->top)
	{
		return;
	}

	HANDLE hwdl = GetStdHandle(STD_OUTPUT_HANDLE);
	Expressions *exp = *(exps->base);
	//Poly *ptr;
	printExpression(exp);
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(hwdl, GREY);
	printf(" (ID: %d)\n", exps->ID);
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(hwdl, WHITE);
	/*
	while (exp != NULL) {
		if (cmd_color == CMD_TRUE)
			SetConsoleTextAttribute(hwdl, NUMBER_COLOR);
		if(exp->coeff != 1 || exp->son == NULL)
			printf("%d", exp->coeff);
		ptr = exp->son;
		
		while (ptr) {
			if (cmd_color == CMD_TRUE)
				SetConsoleTextAttribute(hwdl, VARIABLE_COLOR);
			printf("%c", varTable[ptr->var]);
			if (ptr->degree != 1) {
				if (cmd_color == CMD_TRUE)
					SetConsoleTextAttribute(hwdl,INDEX_COLOR);
				printf("^%d", ptr->degree);
			}
			ptr = ptr->son;
		}
		if (cmd_color == CMD_TRUE)
			SetConsoleTextAttribute(hwdl, OPERATOR_COLOR);
		if (exp->next != NULL) {
			if (exp->next->coeff > 0) {
				printf("+");
			}
			else {
				;
			}
		}
			
		exp = exp->next;
	}
	printf("\n");
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(hwdl, WHITE);
		*/
}

void printExpression(Expressions *exp)
{
	HANDLE hwdl = GetStdHandle(STD_OUTPUT_HANDLE);
	Poly *ptr;
	if (exp == NULL) {
		throwError("N/A", YELLOW);
	}
	while (exp != NULL) {
		if (exp->coeff == -1)
			printf("-");
		if (cmd_color == CMD_TRUE)
			SetConsoleTextAttribute(hwdl, NUMBER_COLOR);
		
		if ((exp->coeff != 1 && exp->coeff != -1) || exp->son == NULL) {
			printf("%d", exp->coeff);
		}
			
		ptr = exp->son;

		while (ptr) {
			if (cmd_color == CMD_TRUE)
				SetConsoleTextAttribute(hwdl, VARIABLE_COLOR);
			printf("%c", varTable[ptr->var]);
			if (ptr->degree != 1) {
				if (cmd_color == CMD_TRUE)
					SetConsoleTextAttribute(hwdl, INDEX_COLOR);
				printf("^%d", ptr->degree);
			}
			ptr = ptr->son;
		}
		if (cmd_color == CMD_TRUE)
			SetConsoleTextAttribute(hwdl, OPERATOR_COLOR);
		if (exp->next != NULL) {
			if (exp->next->coeff > 0) {
				printf("+");
			}
			else {
				;
			}
		}

		exp = exp->next;
	}
	if (cmd_color == CMD_TRUE)
		SetConsoleTextAttribute(hwdl, WHITE);
}

void printExpressionSet_all(ExpressionSets *exps)
{
	ExpressionSetsElem *movealbe_base = exps->base;
	int i = 0;
	while (movealbe_base < exps->top) {
		printf("base + %d: ", i);
		printExpression(*movealbe_base);
		movealbe_base++;
		i++;
	}
}

void printfAllBuffer(ExpresionBuffer *expb)
{
	ExpresionBufferElem *moveable_top;
	int i = 0;
	printf("Current Offset = %d\n", expb->offset);
	printf("Current ID = %d\n", expb->currentID);
	moveable_top = expb->top;
	if (moveable_top == expb->base) {
		printf("No expresson in the buffer\n");
		return;
	}
	while (moveable_top > expb->base) {
		moveable_top--;
		printf("The %d expression: ", i);
		printExpressionSet(*moveable_top);
	}
	printf("\n");
}

int varTableCheck(ExpressionSets *exps, char var)
{
	int i = 0;
	while (exps->varTable[i] != -1) {
		if (exps->varTable[i] == var) {
			return i;
		}
		i++;
	}
	return -1;
}

void expressionSetsVarCpy(ExpressionSets *exps) {
	int i = 0;
	while (varTable[i] != -1) {
		exps->varTable[i] = varTable[i];
		i++;
	}
	while (i < MAXVAR) {
		varTable[i] = -1;
		i++;
	}
}

void expressionSetsVarSync(ExpressionSets *exps) {
	int i = 0;
	while (i < MAXVAR) {
		varTable[i] = exps->varTable[i];
		varValueConfig[i] = false;
		varValue[i] = 1;
		i++;
	}
}
/*
ExpressionSets *expressionSetsVarAdder(ExpressionSets *exps)
{
	int i = 0;
	int j;
	PolyVarType ref[MAXVAR] = { -1 };
	ExpressionSetsElem *moveable_base;
	Expressions *exp, *exp_temp;
	Poly *mpoly;
	exp_temp->next = NULL;

	while (i < MAXVAR && (exps->varTable)[i] != -1) {
		j = 0;
		while ((exps->varTable)[i] != varTable[j] && varTable[j] != -1) {
			j++;
		}
		if (varTable[j] == -1) {
			varTable[j] = (exps->varTable)[i];
			varTable[j + 1] = -1;
		}
		ref[i] = j;
		i++;
	}
	moveable_base = exps->base;
	while (moveable_base < exps->top) {
		exp = *moveable_base;
		while (exp) {
			mpoly = exp->son;
			while (mpoly)
			{
				i = 0;
				mpoly->var = ref[mpoly->var];
				
				mpoly = mpoly->son;
			}
			mpoly = exp_temp->son = exp->son;
			exp->son = NULL;
			while (mpoly) {
				add
				mpoly = mpoly->son;
			}
			exp = exp->next;
		}
		moveable_base++;
	}

}*/

void expressionSetsVarAdder(ExpressionSets *exps)
{
	ExpressionSetsElem *moveable_base;
	Expressions *exp;
	Poly *mpoly, *mpolyHead;

	moveable_base = exps->base;
	while (moveable_base < exps->top) {
		exp  = *moveable_base;
		while (exp) {
			mpoly = mpolyHead = exp->son;
			exp->son = NULL;
			while (mpoly) {
				addNewVariable((exps->varTable)[mpoly->var], exp, mpoly->degree);
				mpoly = mpoly->son;
			}
			freePolyList(mpolyHead);
			exp = exp->next;
		}
		moveable_base++;
	}
	
}

ExpressionSets *expressionSetsDuplicate(ExpressionSets *Source, bool hasID) {
	ExpressionSets *Destination;
	ExpressionSetsElem exps;
	int i = 0;
	ExpressionSetsElem *moveable_base = Source->base;
	if (Source == NULL) {
		return NULL;
	}
	Destination = initExpressiosSets(hasID, NULL);
	for (i = 0; i < MAXVAR; i++) {
		Destination->varTable[i] = Source->varTable[i];
	}
	while (moveable_base < Source->top) {
		exps = expressionDuplicate(*moveable_base);
		pushExpressiosSets(Destination, exps);
		moveable_base++;
	}
	return Destination;
}

ExpressionSets *getExpressionSet(int ID, ExpresionBuffer *expb, bool ifDuplicte, int *ifFromFile, char *name)
{
	ExpresionBufferElem *moveable_base = expb->base;
	ExpressionSets *exps;
	int flag = 0;
	if (name == NULL) {
		while (moveable_base < expb->top) {
			if ((*moveable_base)->ID == ID) {
				flag = 1;
				if (ifDuplicte) {
					exps = expressionSetsDuplicate(*moveable_base);
				}
				else
				{
					exps = *moveable_base;
				}
				if (ifFromFile != NULL) {
					*ifFromFile = 0;
				}
				return exps;
			}
			moveable_base++;
		}
		if (!flag) {
			if (ifFromFile != NULL) {
				*ifFromFile = 1;
			}
			freadExpressionSets(exps, SAVE_AUTO_ID, IOMODE_A, ID, NULL);
			return exps;
		}
	}
	else if(name != NULL)
	{
		while (moveable_base < expb->top) {
			if (strcmp((*moveable_base)->name, name) == 0) {
				flag = 1;
				if (ifDuplicte) {
					exps = expressionSetsDuplicate(*moveable_base);
				}
				else
				{
					exps = *moveable_base;
				}
				if (ifFromFile != NULL) {
					*ifFromFile = 0;
				}
				return exps;
			}
			moveable_base++;
		}
		if (!flag) {
			if (ifFromFile != NULL) {
				*ifFromFile = 1;
			}
			freadExpressionSets(exps, SAVE_AUTO_NAME, IOMODE_A, ID, name);
			return exps;
		}
	}
	return NULL;
}

void pushNumberSignToExpression(const char *s, ExpressionSets *exps, ExpresionBuffer *expb) 
{
	Expressions *mexp;
	ExpressionSets *mexps = NULL;
	ExpressionSetsElem *moveable_base;
	int mID;
	char name[MAX_NAME];
	char c;
	moveable_base = exps->base;
	s++;
	if (isOperator(*s)) {
		mexps = expressionSetsDuplicate(getCurrentBufferExpressionSets(expb), false);
	}
	else if (isNumber(*s)) {
		mID = cmdGetNum_static<int>(s-1);
		if (mID <= 0) {
			throwError("pushNumberSignToExpression::Wrong format of #\n", GREY);
		}
		else {
			freadExpressionSets(mexps, SAVE_AUTO_ID, IOMODE_A, mID, NULL, NULL, NULL, false);
		}
	}
	else if (isLegalExpressionSetNameChar(*s)) {
		nameLegalizeCpy(name, s);
		freadExpressionSets(mexps, SAVE_AUTO_NAME, IOMODE_A, 0, name, NULL, NULL, false);
	}
	if (mexps == NULL) {
		return;
	}
	else {
		//printExpressionSet(mexps);
		expressionSetsVarAdder(mexps);
		moveable_base = mexps->base;
		while (moveable_base < mexps->top) {
			pushExpressiosSets(exps, *moveable_base);
			moveable_base++;
		}
		//it doesn't have to free the stuff it has stored;
		free(mexps->base);
		free(mexps);
	}
}