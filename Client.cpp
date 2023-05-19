#include "Client.h"

string client(string mes_cl)
{
    //int MESSAGE_LENGTH = 1024;
    //int PORT = 7777;
    int socket_file_descriptor, connection;
    struct sockaddr_in serveraddress, client;
    char message[MESSAGE_LENGTH_CL];

    //Создаем сокет
    socket_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_file_descriptor == -1)
    {
        cout << "Создание сокета не удалось!"<<endl;
        return ("-1");
    }
    bzero(&serveraddress, sizeof(serveraddress));
    //Установим адрес сервера
    serveraddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    //Зададим номер порта
    serveraddress.sin_port =htons(PORT_CL);
    //Используем IPv4
    serveraddress.sin_family = AF_INET;
    //Установим соединение с сервером
    connection = connect(socket_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if(connection == -1)
    {
        cout << "Соединение с сервером не удачно."<<endl;
        close(socket_file_descriptor);
        return ("-1");
    }
    //Взаимодействие с сервером
    while(1)
    {
        bzero(message, sizeof(message));
        mes_cl+="~";
        mes_cl.copy(message, mes_cl.length());
        ssize_t bytes = write(socket_file_descriptor, message, sizeof(message));
        bzero(message, sizeof(message));
        //Ждем ответа от сервера
        read(socket_file_descriptor, message,sizeof(message));
        close(socket_file_descriptor);
        mes_cl=message;
        return mes_cl;
    }
}
