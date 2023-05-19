#pragma once
#include <stdio.h>
#include "Admin.h"
#include "Chat.h"
#include "PerCor.h"
#include "Dict.h"


/*
	Зоголовок меню содержит объявление 3-х функций вызываемых базовой функцией.

	administration - функция реалилующая администрирование.
	chat - функция реализующая чат.
	perCor - (от personal correspondence) реализующая личную переписку
	regulation - описание правил пользования программой.
*/

void pauses();
void administration();
void chat();
void perCor();
void regulation();
