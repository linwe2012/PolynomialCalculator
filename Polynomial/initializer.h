#pragma once
#include <iostream>
#include "PolyCalculator.h"
#define PREFERENCE_UPDATE_ID 1

void setMyCurrentConsolefont();
void initIOControl();
void preferenceUpadate(int UpadateMode, int *Para1 = NULL, char *Para2 = NULL);
void firstTimeWelcome(OperatorStack *opStack, FragmentStack *fragStack, ExpresionBuffer *expBuffer);
void initialWelcome();
extern int iniHello;