#pragma once
#include <iostream>
#include <map>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "Message.h"
#include "MessagePC.h"
#include "User.h"
#include "Times.h"

using namespace std;

#define MESSAGE_LENGTH 1024 //максимальныйразмер буфера для данных
#define PORT 7777 //будем использовать этот номер порта

void expCin();

class Server
{
private:
    User user;
    map <const string, User> users_;
    struct sockaddr_in serveraddress, client;
    socklen_t length;
    int sockert_file_descriptor, connection, bind_status, connection_status;
    char message[MESSAGE_LENGTH];


public:
    void miServer();
private:
    string login(string);
    void getUsersFile();
    string reg(string);
    bool getUserByName(const string& name)const;
    bool getUserByLogin(const string& login)const;
    string setUsersFile(string login, string password, string name);
    string setUsersFile(string str);
    string getMessage_File();
    string getMessagePC_File();
    string get_UsersFile();
    string addMessage(const string) const;
    string setMessagePCfile(const string message)const;
    string setMessageFile(const string)const;
    string changAdminPassw(string pass);
    void logServer(string);
    void pauses();
};
