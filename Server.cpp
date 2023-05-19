#include "Server.h"
#include "Menu.h"

void Server::miServer()
{

    //int PORT = 7777;
    //создадим сокет
    sockert_file_descriptor = socket(AF_INET, SOCK_STREAM, 0);
    if (sockert_file_descriptor == -1)
    {
        cout << "Создание сокета неудачно!\n";
        exit(1);
    }
    //bzero(&serveraddress, sizeof(serveraddress));//!!!!!!!!!!!!!
    //
    //serveraddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddress.sin_addr.s_addr = htonl(2130706433);
    //задаем номер порта для связи
    serveraddress.sin_port = htons(PORT);
    //используем IPv4
    serveraddress.sin_family = AF_INET;
    //привяжем сокет
    bind_status = bind(sockert_file_descriptor, (struct sockaddr*)&serveraddress, sizeof(serveraddress));
    if(bind_status == -1)
    {
        cout << "Не удалось выполнить привязку сокета.!"<<endl;
        exit(1);
    }
    //поставим сервер на прием данных
    connection_status = listen(sockert_file_descriptor, 5);
    if (connection_status==-1)
    {
        cout <<"Сокет не может прослушивать новые подключения.!"<<endl;
        exit(1);
    }
    length = sizeof(client);

    //Communication Establishment - Установление связи
    string pref;
    while (true)
    {
        cout <<"Сервер прослушивает новое соединение:"<<" "<< connection << endl;
        string mes_suf="", reply="";
        bzero (message, MESSAGE_LENGTH);
        connection = accept(sockert_file_descriptor, (struct sockaddr*)&client, &length);
        if(connection==-1)
        {
            cout <<"Сервер не может принять данные от клиента.!"<<endl;
            exit(1);
        }

        read (connection, message, sizeof(message));
        //read (connection, message, MESSAGE_LENGTH);
        pref = message[0];
        for (int i=1;i<MESSAGE_LENGTH;i++)
        {
            if (message[i]=='~')break;
            mes_suf=mes_suf+message[i];
        }
        if (pref=="l") 
        {
            string log;
            log = time() + " Запрос от пользователя на логин.";            
            logServer(log);
            reply=login(mes_suf);
            if (reply=="0") log=time() + " Логин пользователя не удачен.";
            if (reply=="b") log=time() + " Пользователь забанен, логин отклонен.";
            if (reply!="0"&&reply!="b") log = time() + " Логин пользователя " + reply + " одобрен.";
            logServer(log);            
        }
        if (pref=="r") 
        {
            string log;
            log = time() + " Запрос от пользователя на регистрацию.";
            cout << log <<endl;
            logServer(log);
            reply=reg(mes_suf);
            if (reply=="0")log = time() + " Регистрация пользователя одобрена.";
            if (reply=="1" || reply=="2") log = time() + " Регистрация пользователя не удачна.";
            logServer(log);
        }
        if (pref=="m") 
        {
            string log;
            log = time() + " Запрос от пользователя содержимого файла переписки.";
            cout << log <<endl;
            logServer(log);
            reply=getMessage_File();
        }
        if (pref=="p") 
        {
            string log;
            log = time() + " Запрос от пользователя содержимого файла личной переписки.";
            cout << log <<endl;
            logServer(log);
            reply=getMessagePC_File();
        }
        if (pref=="u") 
        {
            string log;
            log = time() + " Запрос от пользователя содержимого файла пользователей.";
            cout << log <<endl;
            logServer(log);
            reply=get_UsersFile();
        }
        if (pref=="s") 
        {
            string log;
            log = time() + " Запрос от пользователя на сохранение сообщения в общей переписке.";
            cout << log <<endl;
            logServer(log); 
            reply=addMessage(mes_suf);
        }
        if (pref=="e") 
        {
            string log;
            log = time() + " Запрос от пользователя на сохранение личной переписки.";
            cout << log <<endl;
            logServer(log); 
            reply=setMessagePCfile(mes_suf);
        }
        if (pref=="w") 
        {
            string log;
            log = time() + " Запрос от пользователя на сохранение общей переписки.";
            cout << log <<endl;
            logServer(log); 
            reply=setMessageFile(mes_suf);
        }
        if (pref=="c") 
        {
            string log;
            log = time() + " Запрос администратора на смену пароля.";
            cout << log <<endl;
            logServer(log); 
            reply=changAdminPassw(mes_suf);
        }
        if (pref=="q") 
        {
            string log;
            log = time() + " Запрос от пользователя на сохранение обновленной базы пользователей.";
            cout << log <<endl;
            logServer(log);
            reply=setUsersFile(mes_suf);
        }
       
        bzero (message, MESSAGE_LENGTH);
        int i;
       reply.copy(message, reply.length()+1);
        ssize_t bytes = write(connection, message, sizeof(message));
        // Если передали >= 0 байт, значит пересылка прошла успешно
        if (bytes >= 0)
        {
            cout <<"Данные успешно переданы клиенту!"<<endl;
        }
    }
    //закрываем сокет, завершаем соединение
    close (sockert_file_descriptor);
    return;
}

string Server::login(string message)
{
    int i;
    string login, password, me_;
    for (i=0; i<sizeof(message);i++)
    {
        if (message[i]==' ') break;
        login+=message[i];
    }
    i++;
    for (int j=i;j<sizeof(message);j++) password += message[j];
    getUsersFile();
    bool a = password.compare(users_.find(login)->second.getPassword());
    if(!a)
	{
		return ("0");
	}
	if (users_.find(login)->second.getBan() == "true")
	{
        return ("b");
	}
    return (users_.find(login)->second.getName());
}

void Server::getUsersFile()
{
	char tmp;
	string banTMP;
	string loginTMP;
	string passwordTMP;
	string nameTMP;
	while (true)
	{
		ifstream inf("./Users.txt");
		/*
		повторить операцию, чтобы обеспечить обязательность загрузки базы пользователей.
		*/

		if (!inf.is_open())
		{
			while (true)
			{
				cout << "Неудачная загрузка базы пользователей, наберите любой символ для повтора или \n0 - для продолжения без загрузки базы пользователей.: ";
				pauses();
				break;
			}
			if (tmp == '0') break;
			continue;
		}
        users_.clear();
		while (inf)
		{
			getline(inf, banTMP);
			if (!inf) 
            {
	        	inf.close();
                break;
            }
			getline(inf, loginTMP);
			if (!inf) 
            {
	        	inf.close();
                break;
            }
			getline (inf, passwordTMP);
			if (!inf) 
            {
	        	inf.close();
                break;
            }
			getline (inf, nameTMP);
			User user = User(banTMP, loginTMP, passwordTMP, nameTMP);
			users_.insert(make_pair(loginTMP, user));
		}
		inf.close();
		break;
	}
}

string Server::reg(string message)
{
    int i;
    string login, password, name;
    for (i=0; i<sizeof(message);i++)
    {
        if (message[i]==' ') break;
        login+=message[i];
    }
    i++;
    for (i; i<sizeof(message);i++)
    {
        if (message[i]==' ') break;
        password+=message[i];
    }
    i++;
    for (i;i<sizeof(message);i++) 
    {
        if (message[i]=='~') break;
        name += message[i];
    }
    getUsersFile();
 	if (getUserByName(name) || name == "0" || name == "admin") return ("1");
    if (getUserByLogin(login) || login == "admin" || login == "0")return ("2");
	setUsersFile(login, password, name);
    User user = User("false", login, password, name);
    users_.insert({ login, user });
    string str;
	string mes_cl ="q";
	for (auto& user : users_)
	{
		mes_cl = mes_cl + user.second.getBan() + "\n" + user.second.getLogin() + "\n" + 
		user.second.getPassword() + "\n" + user.second.getName() + "\n";
	}
	setUsersFile(mes_cl);
    return ("0");
}

//проверка имени на повторы.
bool Server::getUserByName(const string& name)const
{
	for (auto it = users_.begin(); it != users_.end(); ++it)
	{
		if (it->second.getName() == name)return true;
	}
	return false;
}

//проверка логина на повторы
bool Server::getUserByLogin(const string& login)const
{
	for (int i = 0; i < users_.size(); ++i);

	for (auto it = users_.begin(); it != users_.end(); ++it)
	{
		if (it->first == login)return true;
	}
	return false;
}

string Server::setUsersFile(string login, string password, string name)
{
    User user = User("false", login, password, name);
    users_.insert({ login, user });
    string str;
    string tmp;
	while (true)
	{
		string mes_cl ="q";
		for (auto& user : users_)
		{
			mes_cl = mes_cl + user.second.getBan() + "\n" + user.second.getLogin() + "\n" + 
			user.second.getPassword() + "\n" + user.second.getName() + "\n";
		}
		return (setUsersFile(mes_cl));
	}
 return "0";
}


string Server::setUsersFile(string str)
{
	char tmp;
	while (true)
	{
		ofstream outf("./Users.txt", ios::out);
		if (!outf)
		{
			while (true)
			{
				cout << "Неудачная попытка сохранить данные, наберите любой символ для повтора или \n0 - для продолжения без загрузки базы пользователей.: ";
				cin >> tmp;
				if (cin.fail())
				{
					expCin();
					outf.close();
					continue;
				}
				break;
			}
			if (tmp == '0') break;
			continue;
		}
        outf << str;
        outf.close();
        getUsersFile();
        break;
	}
	return "0";
}

/*
		getMessagePC_File  -  считывание  содержимого из файла messages.txt и
	возврат результата клиенту
*/
string Server::getMessage_File()
{
    string tmp, str;
    cin.clear();
	ifstream inf("./Messages.txt");
	if (!inf)
	{
        return ("-1");
	}
    while(!inf.eof())
    {
        //inf>>tmp;
        getline(inf, tmp);
        str = str + tmp + "\n";
    }
 	inf.close();
	return str;
}

string Server::getMessagePC_File()
{
    string tmp, str;
    cin.clear();
	ifstream inf("./MessagesPC.txt");
	if (!inf)
	{
        return ("-1");
	}
    while(!inf.eof())
    {
        //inf>>tmp;
        getline(inf, tmp);
        str = str + tmp + "\n";
    }
 	inf.close();
	return str;
}


string Server::get_UsersFile()
{
    string tmp, str;
    cin.clear();
	ifstream inf("./Users.txt");
	if (!inf)
	{
        return ("-1");
	}
    while(!inf.eof())
    {
        inf>>tmp;
        str = str + tmp + "\n";
    }
 	inf.close();
	return str;
}


/*
	  --- ВСПОМОГАТЕЛЬНЫЙ МЕТОД метода showChat ---
		addMessage - добавление нового сообщения в переписку, с добавлением
	данного сообщения в файл messages.txt и в контейнер message_.
*/
string Server::addMessage(const string message)const
{
    string user, txt;
    int i;
    for (i=0; i<sizeof(message);i++)
    {
        if (message[i]==' ') break;
        user+=message[i];
    }
    i++;
    for (i;i<sizeof(message);i++) 
    {
        if (message[i]=='~')break;
        txt += message[i];
    }
	while (true)
	{
		ofstream outf("./Messages.txt", ios::app);
		if (!outf.is_open())
		{
			cout << "Неудачная попытка сохранить сообщение, наберите любой символ для повтора или \n0 - для продолжения без отправки сообщения.: ";
            break;
		}
		outf << "false\n" << time() << "\n" << user << "\n" << txt << "\n";
		outf.close();
		break;
	}
    return  "1";
}

string Server::setMessagePCfile(const string str)const
{
    cin.clear();
	ofstream outf("./MessagesPC.txt", ios::out);
    outf << str;
    outf.close();
	return "0";
}

string Server::setMessageFile(const string str) const
{
    cin.clear();
	ofstream outf("./Messages.txt", ios::out);
    outf << str;
    outf.close();
	return "0";
}


string Server::changAdminPassw(string pass)
{
    getUsersFile();
    string txt;
    for (int i=0;i<sizeof(pass);i++) 
    {
        if (pass[i]=='~')break;
        txt += pass[i];
    }
    pass = txt;
    users_.find("admin")->second.setPassword(pass);
	char tmp;
	while (true)
	{
		ofstream outf("./Users.txt");
		if (!outf)
		{
            break;
		}
	    for (auto it = users_.begin(); it != users_.end(); ++it)
        {
            outf << it->second.getBan() << "\n" << it->second.getLogin() << "\n";
            outf << it->second.getPassword() << "\n" << it->second.getName() << "\n";
        }
        outf.close();
		break;
	}
    return "0";
}


void Server::logServer(string log)
{
	while (true)
	{
		ofstream outf("./logServer.txt", ios::app);
		/*
		повторить операцию, чтобы обеспечить обязательность сохранения лога.
		*/

		if (!outf)
		{
			char tmp;
			while (true)
			{
				cout << "Не удачное открытие лог файла, наберите любой символ для повтора или \n0 - для продолжения без сохранения лога.: ";
                cin >> tmp;
				if (cin.fail())
				{
					expCin();
					outf.close();
					continue;
				}
				break;
			}
			if (tmp == '0') break;
			continue;
		}
        outf << log << "\n";
        outf.close();
		break;
	}
}

void expCin()
{
	cin.clear();
	cin.ignore(32767, '\n');
	cout << "Ошибка ввода, повторите попытку.\n";
	cout << cin.get();
	cin.clear();
	cin.ignore(32767, '\n');
};

void Server::pauses()
{
	char a;
	cin.clear();
	cin.ignore(32767, '\n');
	cout << endl << "Для продолжени нажмите любую клавишу.";
	cout << cin.get();//pause
}