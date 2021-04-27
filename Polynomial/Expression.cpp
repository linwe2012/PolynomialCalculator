#include "Expression.h"
#include "PolishReverse.h"
#include <stdlib.h>
#include "ExpressionSet.h"
#include "PolyCalculator.h"
int newVar(char v, bool *isNewVar);
PolyType getNextNum(const char *s, int *leap);



char varTable[MAXVAR];
bool varValueConfig[MAXVAR];
PolyType varValue[MAXVAR];

int ExpressionSize = sizeof(Expressions);
int PolySize = sizeof(Poly);

/*
Expressions *newExpression(char *s, char *head){
	Expressions *newexp = NULL;
	//Poly *polyptr, *polyptr_last;
	int leap;
	//case like abc^8abc or abc^a88
	if (!isStringEnd(*(s + 1)) && !isOperator(*(s + 1)) && isNumber(*s) != isNumber(*(s + 1)) ||
		!isStringEnd(*(s + 2)) && isVariable(*(s + 1)) && *(s + 2) == '^') {
		if (isNumber(*s)) {
			newexp = (Expressions *)malloc(ExpressionSize);
			newexp->coeff = getNextNum(s, &leap);
			newexp->next = NULL;
			newexp->son = NULL;
		}
		else {
			printf("Illegal Expression: %c", *s);
		}
	}
	else
	{
		newexp = (Expressions *)malloc(ExpressionSize);
		newexp->coeff = 1;
		newexp->next = NULL;
		newexp->son = NULL;
		while (!isOperator(*s)) {
			if (isNumber(*s)) {
				newexp->coeff *= getNextNum(s, &leap);
				if (newexp->coeff == 0) {
					freePolyList(newexp->son);
					return newexp;
				}
			}
			else {

			}
			s++;
		}
	}

	return newexp;
}
*/

Expressions *newExpression(const char *s) {
	Expressions *newexp;
	int leap;
	newexp = (Expressions *)malloc(ExpressionSize);
	newexp->next = NULL;
	newexp->son = NULL;
	if (isNumber(*s)) {
		newexp->coeff = getNextNum(s, &leap);
	}
	else {
		while (isVariable(*s) && *s != '#') {
			newexp->coeff = 1;
			addNewVariable(*s, newexp);
			s++;
		}
	}
	return newexp;
}


int newVar(char v, bool *isNewVar)
{
	int i = 0;
	while (varTable[i] != -1) {
		if (varTable[i] == v) {
			*isNewVar = false;
			return i;
		}
		i++;
	}
	varTable[i] = v;
	varTable[i + 1] = -1;
	*isNewVar = true;
	return i;
}

PolyType getNextNum(const char *s, int *leap) {
	PolyType num = 0;
	PolyType denominator = 1;
	PolyType isBeforeDot = 10;
	*leap = 0;

	while (*s >= '0' && *s <= '9') {
		num = num * isBeforeDot + (*s - '0') / denominator;
		if (*s == '.' || denominator > 1) {
			denominator = denominator * 10;
			isBeforeDot = 1;
		}
		*leap++;
		s++;
	}
	return num;
}

void freePolyList(Poly *&head) {
	Poly *ptr = head;
	while (head) {
		ptr = head;
		head = head->son;
		free(ptr);
	}
}

void freeExpression(Expressions *&head) {
	Expressions *exp = head;
	while (head) {
		freePolyList(((head)->son));
		exp = head;
		head = (head)->next;
		free(exp);
	}
}

int addNewVariable(const char var, Expressions *exp, PolyVarType degree) {
	bool isNewVar;
	int varPriority = newVar(var, &isNewVar);
	int flag = 0;
	Poly *newVarNode, *ptr;
	Poly *head = exp->son;

	newVarNode = (Poly *)malloc(PolySize);
	newVarNode->degree = degree;
	newVarNode->son = NULL;
	newVarNode->var = varPriority;

	if (isNewVar == true) {
		if (head == NULL) {
			exp->son = newVarNode;
		}
		else {
			while (head->son != NULL) {
				head = head->son;
			}
			head->son = newVarNode;
		}
		
	}
	else {
		if (head == NULL) {
			exp->son = newVarNode;
		}
		else if (head->var > varPriority) {
			newVarNode->son = head;
			exp->son = newVarNode;
		}
		else if (head->var == varPriority) {
			head->degree += degree;
			free(newVarNode);
			return 0;
		}
		else {
			ptr = head->son;
			while (ptr) {
				if (head->var == var) {
					head->degree += degree;
					free(newVarNode);
					flag = 1;
					break;
				}
				else if (ptr->var > varPriority && head->var < varPriority) {
					newVarNode->son = ptr;
					head->son = newVarNode;
					flag = 1;
					break;
				}
				else {
					head = ptr;
					ptr = ptr->son;
				}
			}
			if (flag == 0) {
				head->son = newVarNode;
			}
		}
		
	}
	return 0;
}

//debug before use it
Poly *polyDuplicate(Poly *Source) {
	Poly *ptr, *Destination;
	if (Source == NULL) {
		return NULL;
	}
	Destination = (Poly *)malloc(PolySize);
	ptr = Destination;
	(Destination)->degree = Source->degree;
	(Destination)->var = Source->var;
	(Destination)->son = NULL;
	Source = Source->son;
	while (Source != NULL) {
		ptr->son = (Poly *)malloc(PolySize);
		ptr = ptr->son;
		ptr->var = Source->var;
		ptr->degree = Source->degree;
		Source = Source->son;
	}
	ptr->son = NULL;
	return (Destination);
}

//debug before use it
Expressions *expressionDuplicate(Expressions *Source) {
	Expressions *exp, *Destination;
	if (Source == NULL) {
		return NULL;
	}
	Destination = (Expressions *)malloc(ExpressionSize);

	(Destination)->coeff = Source->coeff;
	exp = (Destination);
	(Destination)->son = polyDuplicate(Source->son);
	Source = Source->next;
	while (Source) {
		exp->next = (Expressions *)malloc(ExpressionSize);
		exp = exp->next;
		exp->coeff = Source->coeff;
		exp->son = polyDuplicate(Source->son);
		Source = Source->next;
	}
	exp->next = NULL;
	return Destination;
}

void expressionSorter(Expressions *&exp)
{
	Expressions *res = NULL, *myexp, *resHead = NULL;
	Expressions *max;
	Expressions *ptr1;//pointer before myexp
	Expressions *ptr2;//pointer before max
	int priority;

	while (exp != NULL) {
		//find the max and node before it
		ptr2 = max = ptr1 = exp;
		myexp = ptr1->next;
		while (myexp != NULL) {
			if (myexp->coeff == 0) {
				freePolyList(myexp->son);
			}
			if ((priority = compare(myexp, max)) == 1) {
				max = myexp;
				ptr2 = ptr1;
			}
			//merge if equal
			else if(priority == 0)
			{
				max->coeff += myexp->coeff;
				ptr1->next = myexp->next;
				freePolyList(myexp->son);
				free(myexp);
				myexp = ptr1->next;
				continue;
			}
			ptr1 = myexp;
			myexp = myexp->next;
		}
		
		if (resHead == NULL) {
			res = (Expressions *)malloc(ExpressionSize);
			resHead = res;
		}
		else {
			res->next = (Expressions *)malloc(ExpressionSize);;
			res = res->next;
		}
		res->coeff = max->coeff;
		//it equals to max.son. Ergo we only have to free the ptr1 instead of myexp.son
		res->son = max->son;
		//max is head itself
		if (max == exp) {
			ptr1 = exp;
			exp = exp->next;
			free(ptr1);
		}
		else {
			ptr2->next = max->next;
			free(max);
		}
	}
	res->next = NULL;
	if (resHead->coeff == 0) {
		if(resHead->next != NULL)
			freeExpression(resHead->next);
		if (resHead->son != NULL)
			freePolyList(resHead->son);
	}
	ptr1 = resHead;
	res = resHead->next;
	while (res != NULL) {
		if (res->coeff == 0) {
			ptr1->next = NULL;
			freeExpression(res);
			break;
		}
		ptr1 = res;
		res = res->next;
	}
	exp = resHead;
}

void initVar() {
	for (int i = 0; i < MAXVAR; i++) {
		varValue[i] = 0;
		varValueConfig[i] = false;
		varTable[i] = -1;
	}
}

int isVarInTable(char c)
{
	int i = 0;
	while (varTable[i] != -1 && i < MAXVAR) {
		if (varTable[i] == c) {
			return i;
		}
		i++;
	}
	return -1;
}

