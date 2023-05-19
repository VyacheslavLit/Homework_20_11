﻿#include "Menu.h"

void pauses()
{
	char a;
	cin.clear();
	cin.ignore(32767, '\n');
	cout << endl << "Для продолжени нажмите любую клавишу.";
	cout << cin.get();//pause
	cin.clear();
}

/*
	определение 3-х функций вызываемых базовой функцией.

	administration - функция реалилующая администрирование.
	chat - функция реализующая чат.
	perCor - (от personal correspondence) реализующая личную переписку
*/


/*
	определение функции администрирования

	1. активация класса Admin - основной класс на котором реализовано администрирование.
	2. активация класса User - класс содержащий информацию о пользователях.
	3. активация метода LoginMenu входящего в класс Admin, который позволяет залогиниться.
	4. при удачном входе администратора активируется adminMenu (меню администратора).

	при окончании работы, самопроизвольный возврат в базовую функцию.
*/
void administration()
{
	Admin Admin;
	User user;
	Admin.loginMenu();
	if (Admin.getLogin()) Admin.adminMenu();
}

/*
	определение функции общей переписки

	1. активация класса Chat - основной класс на котором реализована общая переписки.
	2. активация класса User - класс содержащий информацию о пользователях.
	3. активация метода LoginMenu входящего в класс Chat, который позволяет залогиниться и зарегистрировать нового пользователя.
	4. при удачном входе пользователя активируется showChat (общий чат).

	при окончании работы, самопроизвольный возврат в базовую функцию.
*/
void chat()
{
	Chat Chat;
	User user;
	cin.clear();
	Chat.loginMenu();
	if (Chat.getMe() !="")Chat.showChat();
}

/*
	определение функции личной переписки

	1. активация класса PerCor - основной класс на котором реализована личная переписка.
	2. активация класса User - класс содержащий информацию о пользователях.
	3. активация метода LoginMenu входящего в класс perCor, который позволяет залогиниться и зарегистрировать нового пользователя.
	4. при удачном входе пользователя активируется userMenuPC (пользовательское меню), позволяющее выбрать собеседника
	5. при выборе собеседника активизируется showPC (личный чат), переписка с выбранным пользователем.

	при окончании работы, самопроизвольный возврат в базовую функцию.
*/
void perCor()
{
	PerCor perCor;
	User user;
	perCor.loginMenuPC();
	if (perCor.getMe()!="") perCor.userMenuPC();

	if (perCor.getParty() != "") perCor.showPC();
}

//Описание правил пользования программой
void regulation()
{
	char tmp;
	system("clear");
	cout << "\t\t--- Правила пользования программой ---\n1. Закрытые логины и имена при регистрации:\n\t(admin)\n\t(0)\n2. В сообщениях запрещено:\n";
	cout << "\tОскорблять\n\tПризывать к насилию\n3. Возможные наказания за нарушения:\n\tБан по логину\n\tУдаление сообщения\n\n\n";
	cin.clear();
	pauses();
}

