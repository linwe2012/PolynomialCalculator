#include "PolyCalculator.h"
#include "ExpressionSet.h"
#include "PolishReverse.h"
#include "command.h"
#include "errorDealer.h"
#include "IOControl.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

//-1:p1 behind p2
//0: p1 = p2
//1:p1 before p2; p1 higher priority
int compare(Expressions* p1, Expressions* p2) {
	Poly*ptr1, *ptr2;
	int flag = 0;
	ptr1 = p1->son;
	ptr2 = p2->son;
	if (ptr1 == NULL && ptr2 == NULL)
		return 0;
	else if (ptr1 == NULL)
		return -1;
	else if (ptr2 == NULL)
		return 1;
	while (ptr1->degree == ptr2->degree && ptr1->var == ptr2->var) {
		if (ptr1->son == NULL && ptr2->son == NULL)
			return 0;
		else if (ptr1->son == NULL)
			return -1;
		else if (ptr2->son == NULL)
			return 1;
		else {
			ptr1 = ptr1->son;
			ptr2 = ptr2->son;
		}
	}
	if (ptr1->var != ptr2->var) {
		flag = ptr2->var - ptr1->var;
	}
	else {
		flag = ptr1->degree - ptr2->degree;
	}
	return flag;
}
/*
void copyExp(Expressions* p1, Expressions* p2) {
	Poly*ptr1, *ptr2, *p;
	p = (Poly*)malloc(sizeof(Poly));
	p2->son = p;
	p2->coeff = p1->coeff;
	ptr1 = p1->son;
	ptr2 = p2->son;
	while (ptr1 != NULL) {
		ptr2->degree = ptr1->degree;
		ptr2->var = ptr1->var;
		p = (Poly*)malloc(sizeof(Poly));
		p->son = NULL;
		ptr2->son = p;
		ptr2 = ptr2->son;
		ptr1 = ptr1->son;
	}
}
//copy from p1 to p2;
Expressions* findtail(Expressions* exp1) {
	Expressions* tail = exp1;
	while (tail->next != NULL) {
		tail = tail->next;
	}
	return tail;
}
*/

void add(Expressions* &exp1, Expressions* &exp2)
{
	Expressions*p1, *p2, *exp1_next = NULL;
	int temp;
	Expressions *exp1_dup = exp1;
	//make sure exp1 has a higher or equal priority
	if (exp1 && exp2 && compare(exp2, exp1) > 0) {
		p1 = exp1;
		exp1 = exp2;
		exp2 = p1;
		exp1_dup = exp1;
	}
	//merge all the same priority at the head
	else {
		while (exp1_dup && exp2 && compare(exp2, exp1_dup) == 0) {
			exp1_dup->coeff += exp2->coeff;
			p2 = exp2;
			exp2 = exp2->next;
			freePolyList((p2->son));
			free(p2);
		}
		//printf("start¡® exp1 = exp2  ");
	}
	p1 = exp1;
	for (;exp1_dup != NULL && exp1_dup->next && exp2;) {
		exp1_next = exp1_dup->next;
		//printf("for loop ");
		while (exp2 && (temp = compare(exp2, exp1_next)) >= 0) //exp2 has higher or equal priority
		{
			//printf("while loop ");
			//same: merge & free
			if (temp == 0) {
				exp1_next->coeff += exp2->coeff;
				p2 = exp2;
				exp2 = exp2->next;
				freePolyList((p2->son));
				free(p2);
				//printf("exp1 = exp2  ");
			}
			//different: absorb into exp1
			else {
				exp1_dup->next = exp2;
				exp1_dup = exp1_dup->next;
				exp2 = exp2->next;
			}
		}
		//exp1.next redirect to original next
		exp1_dup->next = exp1_next;
		if (p1 == exp1_dup) {
			exp1_dup = exp1_dup->next;
		}
		p1 = exp1_dup;
	}
	if (exp2 != NULL && exp1_dup) {
		exp1_dup->next = exp2;
	}
	if(exp1 != NULL)
		expressionZeroEliminator(exp1);
}

void sub(Expressions* &exp1, Expressions* &exp2)
{
	Expressions *exp = exp2;
	while (exp) {
		exp->coeff = -exp->coeff;
		exp = exp->next;
	}
	add(exp1, exp2);
}
/*
void add(Expressions* exp1, Expressions* exp2) {
	Expressions*p1, *p2, *p, *tail;
	
	tail = findtail(exp1);
	for (p2 = exp2; p2 != NULL; p2 = p2->next) {
		
		for (p1 = exp1; p1 != NULL; p1 = p1->next) {
			if (compare(p1, p2) >) {
				p1->coeff += p2->coeff;
				p2->coeff = 0;

				break;
			}
			p = p1;
		}
	}
	
	for (p2 = exp2; p2 != NULL; p2 = p2->next) {
		if (p2->coeff != 0) {
			p = (Expressions *)malloc(ExpressionSize);
			copyExp(p2, p);
			tail->next = p;
			tail = tail->next;
		}
	}
}*/
	//add exp2 to exp1;

int addVariableToExpression(PolyType degree, PolyVarType var,Expressions *exp) {

	int flag = 0;
	Poly *newVarNode, *ptr;
	Poly *head = exp->son;

	if (head == NULL) {
		newVarNode = (Poly *)malloc(PolySize);
		newVarNode->degree = degree;
		newVarNode->son = NULL;
		newVarNode->var = var;
		exp->son = newVarNode;
	}
	else if (head->var > var) {
		newVarNode = (Poly *)malloc(PolySize);
		newVarNode->degree = degree;
		newVarNode->var = var;
		newVarNode->son = head;
		exp->son = newVarNode;
	}
	else if (head->var == var) {
		head->degree += degree;
		return 0;
	}
	else {
		ptr = head->son;
		while (ptr) {
			if (head->var == var) {
				head->degree += degree;
				flag = 1;
				return 0;
				break;
			}
			else if (ptr->var > var && head->var < var) {
				newVarNode = (Poly *)malloc(PolySize);
				newVarNode->degree = degree;
				newVarNode->var = var;
				newVarNode->son = ptr;
				head->son = newVarNode;
				flag = 1;
				return 0;
				break;
			}
			else {
				head = ptr;
				ptr = ptr->son;
			}
		}
		if (head->var == var && flag == 0) {
			head->degree += degree;
		}
		else if (flag == 0) {
			newVarNode = (Poly *)malloc(PolySize);
			newVarNode->degree = degree;
			newVarNode->son = NULL;
			newVarNode->var = var;
			head->son = newVarNode;
		}
	}

	return 0;
}

Expressions * mul(Expressions *exp1, Expressions *exp2)
{
	int size1 = sizeof(Expressions);
	//define size of structs

	Expressions *exp3 = NULL, *p1, *p2, *p3, *head, *temp_head;
	Poly *ptr1, *ptr2; //*ptr3, *head, *tail;
	//definition of points
	p3 = NULL;
	head = NULL;
	for (p1 = exp1; p1 != NULL; p1 = p1->next) {
		temp_head = exp3 = NULL;
		for (p2 = exp2; p2 != NULL; p2 = p2->next) {
			p3 = (Expressions *)malloc(ExpressionSize);
			//create new space for new list			
			p3->coeff = p1->coeff*p2->coeff;
			p3->next = NULL;
			p3->son = NULL;

			ptr1 = p1->son;
			ptr2 = p2->son;
			while (ptr1) {
				addVariableToExpression(ptr1->degree, ptr1->var, p3);
				ptr1 = ptr1->son;
			}
			while (ptr2) {
				addVariableToExpression(ptr2->degree, ptr2->var, p3);
				ptr2 = ptr2->son;
			}
			if (exp3 == NULL)
			{
				temp_head = exp3 = p3;
			}
			else
			{
				exp3->next = p3;
				exp3 = p3;
			}
			
		}
		if (head == NULL)
			head = temp_head;
		else
			add(head, temp_head);
	}
	expressionZeroEliminator(head);
	return head;
}

/*
void sub(Expressions* exp1, Expressions* exp2) {
	Expressions*p1, *p2, *p, *tail;
	tail = findtail(exp1);

	for (p2 = exp2; p2 != NULL; p2 = p2->next) {
		for (p1 = exp1; p1 != NULL; p1 = p1->next) {
			if (compare(p1, p2)) {
				p1->coeff -= p2->coeff;
				p2->coeff = 0;
				break;
			}
		}
	}
	for (p2 = exp2; p2 != NULL; p2 = p2->next) {
		if (p2->coeff != 0) {
			p2->coeff = -p2->coeff;
			p = (Expressions *)malloc(ExpressionSize);
			copyExp(p2, p);
			tail->next = p;
			tail = tail->next;
		}
	}
}
*/

void expressionZeroEliminator(Expressions *&expHead)
{
	Expressions *exp = expHead;
	Expressions *ptr;
	if (exp == NULL) {
		return;
	}
	else if (exp->coeff == 0) {
		while (exp->coeff == 0) {
			expHead = expHead->next;
			freePolyList(exp->son);
			free(exp);
			exp = expHead;
			if (expHead == NULL) {
				return;
			}
		}
		
	}
	ptr = exp->next;
	
	while (ptr)
	{
		if (ptr->coeff == 0) {
			exp->next = ptr->next;
			freePolyList(ptr->son);
			free(ptr);
			ptr = exp->next;
		}
		else {
			exp = ptr;
			ptr = ptr->next;
		}
		
	}
}

void getNumberSignContent(char *s, ExpressionSets *&exps, ExpresionBuffer *expb)
{
	int mID;
	char name[MAX_NAME];
	int i;
	s++;
	if (isNumber(*s) == true) {
		mID = cmdGetNum<int>(s);
		if (mID <= 0 || mID > globe_ID) {
			throwError("getNumberSignContent::ID exceeds limit\n", GREY);
			return;
		}
		exps = getExpressionSet(mID, expb, true);
	}
	else if (isVariable(*s)) {
		i = 0;
		while ((isVariable(*s)) && i < MAX_NAME) {
			name[i++] = *s;
		}
		exps = getExpressionSet(0, expb, true);
	}
}


int analysisString(char *s) {
	int temp;
	if (cmd_autoCorrect == CMD_TRUE) {
		expressionAutoCorrector(s);
	}
	temp = parentheseCherker(s);
	if (cmd_autoParenthese == CMD_TRUE) {
		if ((temp = parentheseAutoAdder(s, temp, MAX_BUFFER)) == -1) {
			return -1;
		}
		else if (temp == 1)
		{
			throwError("analysisString::Elements are auto added. the Expresion is currently:\n", GREY);
			puts(s);
		}
	}
	else if (temp != 0) {
		if (temp > 0)
			throwError("analysisString::Expected right parenthese ')'.\n", GREY);
		else
			throwError("analysisString::Expected left parenthese '('.\n", GREY);
		return -1;
	}

	if (cmd_autoCorrect == CMD_FALSE || cmd_autoParenthese == CMD_FALSE) {
		if (stringLegalchecker(s) != 0) {
			throwError("analysisString::The expression contains illegal character. Input again or turn on auto-corrector\n", GREY);
			return -1;
		}
		else {
			;
		}
	}
	return 0;
}




//1+2x+3x^2+4x
int cal(FragmentStack *frag, ExpresionBuffer *expb)
{
	FragmentType *moveable_Base;
	static Expressions *exp1, *exp2, *exp_temp, *exp3;
	ExpressionSets *exps;
	static ExpressionSets *expStack;
	char c;
	int num;
	initVar();
	cleanUpHalfTime(true, &exp1, &exp2, &exp_temp, &exp3, &expStack);
	exps = initExpressiosSets(true, NULL);
	expStack = initExpressiosSets(false, NULL, 5);
	moveable_Base = frag->base;
	/*
	exp_temp = newExpression(*moveable_Base);
	pushExpressiosSets(expStack, exp_temp);
	moveable_Base++;*/
	while ((moveable_Base < frag->top)) {
		if (isOperator(**moveable_Base)) {
			c = **moveable_Base;
			if (c == '^') {
				exp2 = popExpressiosSets(expStack);
				exp1 = popExpressiosSets(expStack);
				num = exp2->coeff;
				freeExpression(exp2);
				if (num == 1) {
					pushExpressiosSets(expStack, exp1);
				}
				else if (exp1->next == NULL && exp1->son->son == NULL) {
					exp1->son->degree *= num;
					exp1->coeff = int(pow(exp1->coeff, num));
					pushExpressiosSets(expStack, exp1);
				}
				else {
					//exp2 = expressionDuplicate(exp1);
					exp3 = exp2 = mul(exp1, exp1);
					//printf("(^)expStack = ");
					//printExpressionSet_all(expStack);
					//printf("^\n");

					for (; num > 2; num--) {
						exp3 = mul(exp2, exp1);
						freeExpression(exp2);
						exp2 = exp3;
					}
					pushExpressiosSets(expStack, exp3);
				}


			}
			else if (c == '+') {
				exp2 = popExpressiosSets(expStack);
				exp1 = popExpressiosSets(expStack);
				//printf("prepare add ");
				add(exp1, exp2);
				//printf("add finish");
				//printf("(+)exp1 = ");
				//1+(x+2y)^2
				//printExpression(exp1);
				pushExpressiosSets(expStack, exp1);

				//printf("(+)expStack = ");
				//printExpressionSet_all(expStack);

				exp1 = NULL;
				//printf("+\n");
			}
			//1+(5x+6)-7abc(dsjhhu+sgh)
			else if (c == '-') {
				exp2 = popExpressiosSets(expStack);
				exp1 = popExpressiosSets(expStack);
				sub(exp1, exp2);
				pushExpressiosSets(expStack, exp1);

				//printf("(-)expStack = ");
				//printExpressionSet_all(expStack);
				//printf("-\n");
			}
			else if (c == '*') {
				exp2 = popExpressiosSets(expStack);
				exp1 = popExpressiosSets(expStack);
				exp3 = mul(exp1, exp2);
				pushExpressiosSets(expStack, exp3);

				//printf("(*)expStack = ");
				//printExpressionSet_all(expStack);

				freeExpression(exp1);
				freeExpression(exp2);
				exp3 = NULL;
				//printf("*\n");
			}
			else if (c == '~') {
				exp1 = (*(expStack->top - 1));
				while (exp1) {
					exp1->coeff = -exp1->coeff;
					exp1 = exp1->next;
				}
			}
			
		}
		else {
			if (**moveable_Base == '#') {
				pushNumberSignToExpression(*moveable_Base, expStack, expb);
			}
			else {
				exp_temp = newExpression(*moveable_Base);
				pushExpressiosSets(expStack, exp_temp);
			}
			
			//printf("(add)expStack = ");
			//printExpressionSet_all(expStack);
			//printf("size = %d  \n", expStack->top - expStack->base);
		}
		moveable_Base++;
	}
	pushExpressiosSets(exps, *(expStack->base));
	expressionSetsVarCpy(exps);
	//lift its base so that the expression at the base won't be freed
	(expStack->base)++;
	freeExpressionSets(expStack);
	printExpressionSet(exps);
	pushExpressionBuffer(expb, exps, true);
	//printBufferCurrentOffset(expb);
	//cmdDraw(0, 1, 1, 50, 0, 0, exps);
	return 0;
}