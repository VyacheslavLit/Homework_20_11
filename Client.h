#pragma once
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/acct.h>
#include <arpa/inet.h>

using namespace std;

#define MESSAGE_LENGTH_CL 1024 //Максимальный размер буфера для данных
#define PORT_CL 7777 //будем использовать этот номер порта


string client(string message);

