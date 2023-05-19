#include <iostream>
#include <ctime>
#include <fstream>
#include <iterator>
#include <string.h>
#include "Chat.h"
#include "PerCor.h"
#include "Client.h"

/*
	  --- ОСНОВНОЙ МЕТОД класса переписки Chat ---
		LoginMenu - вход и регистрацию пользователя.
		В данном методе предоставлется выбор и реализация Входа в переписку
	под  своим  логином  и  паролем, регистрация нвого пользователя и выход
	в предыдущее меню.
		Определяется    значение    указателя    currentUser_   (корректный
	пользователь)   текущий   пользователь,  от  лица  которого  происходит
	общение.
		База   пользователей   загружается   из   файла   User.txt,   новые
	пользователи добавляются в данный файл.
*/
void Chat::loginMenu()
{
	string operation;
	while (true)
	{
		system("clear");
		cout << "Логин меню:"<<endl;
		cout << "1. Вход.\n2. Регистрация.\n0. Выход в предыдущее меню.\n";
		cin >> operation;
		cin.clear();
		if (cin.fail())
		{
			expCin();
			continue;
		}
		if (operation == "1")
		{
			login();
			break;
		}
		//При регистрации отрабатываются исключения на повтор имён и логинов.
		if (operation == "2")
		{
			try
			{
				registration();
			}
			catch (const exception& e)
			{
				cout << e.what() << endl;
				pauses();
			}
			continue;
		}
		if (operation == "0")
		{
			break;
		}
		cout << "Выбор не пределен. Повторите попытку.\n";
		pauses();
		continue;
	}
}

/*
	  --- ОСНОВНОЙ МЕТОД класса переписки Chat ---
		showChat  -  вывод  на  экран  переписки с возможностью продолжения
	переписки.
		В  данном  методе  предоставлен  бесконечный цикл, который  выводит
	переписку   и  дает  возможность  написать  новое  сообщение,  обновить
	переписку, чтобы проверить новые сообщения.
*/
void Chat::showChat()
{
	string txt, tmp;
	system("clear");
	/*
	Применение бесконечного цикла обеспечивает непрерывнсть работы
	и возможность обновлять переписку.
	*/
	while (true)
	{
		getMessage_File();
		chat();
		cout << "Введите сообщение или: 1 обновить переписку, 0 выйти из переписки.\n";
		getline(cin, txt);
		if (cin.fail())
		{
			expCin();
			continue;
		}
		if (txt == "1")continue;
		if (txt == "0")break;
		if (!txt.empty()) 
		{
			Message message = Message("false", time(), me_ , txt);
			message_.push_back(message);
			setMessage_File();	
		}
	}
}

/*
	  --- ОСНОВНОЙ МЕТОД класса переписки Chat ---
		showChatDict  -  вывод  на  экран  переписки с возможностью продолжения
	переписки с использованием словаря.
		В  данном  методе  предоставлен  бесконечный цикл, который  выводит
	переписку   и  дает  возможность  написать  новое  сообщение (возможно с
	использованием словаря),  обновить 	переписку, чтобы проверить новые сообщения.
*/

void Chat::showChatDict(Dict dict)//const
{
	string mes;
	char tmp;
	system("clear");
	getMessage_File();
	/*
	Применение бесконечного цикла обеспечивает непрерывнсть работы
	и возможность обновлять переписку.
	*/
	while (true)
	{
		system("clear");
		chat();
		mes = dict.messeng();
		cout << "Продолжить переписку любая буква, 0 выйти из переписки.\n";
		cin >> tmp;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(32767, '\n');
		}
		if (tmp == '0')break;
	}
}

/*
	  --- ВСПОМОГАТЕЛЬНЫЙ МЕТОД  ---
		setMessage_File - сохранение пользователей в файл Users.txt.
*/

void Chat::setMessage_File()
{
	string mes_cl ="w";
	for (auto& mess : message_)
	{
		mes_cl = mes_cl + mess.getDelete() + "\n" + mess.getTime() + "\n" + 
		mess.getFrom() + "\n" + mess.getText() + "\n";
	}
	string tmp=client(mes_cl);
}

/*
	  --- ВСПОМОГАТЕЛЬНЫЙ МЕТОД метода LoginMenu ---
		getUsersFile - загрузка пользователей из файла Users.txt.
		В  даном  методе  мы  заполняем  вектор  User,  данными  считанными
	из  файла  User.txt.  При  этом  происходит  проверка на доступ к файлу
	в случае если он открыт другим пользователем.
*/
void Chat::getUsersFile()
{
	users_.clear();
	string banTMP;
	string loginTMP;
	string passwordTMP;
	string nameTMP;
	while (true)
	{
		system("clear");
		string inf = client("u");
		if (inf=="-1")
		{
			cout << "Ошибка получения данных о пользователях, повторите попытку."<<endl;
			pauses();
			continue;
		}

		while (inf!="")
		{
			banTMP.clear();
			loginTMP.clear();
			passwordTMP.clear();
			nameTMP.clear();

			banTMP.append(inf, 0, inf.find("\n"));
			inf.erase(0,inf.find("\n")+1);
			if (inf=="") break;
			loginTMP.append(inf, 0, inf.find("\n"));
			inf.erase(0,inf.find("\n")+1);
			if (inf=="") break;
			passwordTMP.append(inf, 0, inf.find("\n"));
			inf.erase(0,inf.find("\n")+1);
			if (inf=="") break;
			if (inf.find("\n")!= -1) 
			{
				nameTMP.append(inf, 0, inf.find("\n"));
				inf.erase(0,inf.find("\n")+1);
			}else 
			{
				nameTMP = inf;
				inf.clear();
			}
        	User user = User(banTMP, loginTMP, passwordTMP, nameTMP);
        	users_.insert({ loginTMP, user });
		}
		break;
	}
}

/*
	  --- ВСПОМОГАТЕЛЬНЫЙ МЕТОД метода LoginMenu ---
		login - вход в прорамму по логину и паролю.
		В  даном  методе  пользователю дается возможность ввести свой логин
	и пароль. Производится их проверка, при наличии такой комбинации в базе
	пользователей     выбирается    текущий    пользователь    currentUser_
	и  присваивается  переменной  me_  имя текущео пользователя. если такой
	логин   отсутсвует   или  пароль  не  соответствует  введеному  логину,
	выводится ошибка, и предложение повториь ввод.
*/
void Chat::login() {
	string login, password;
	system("clear");
	do
	{
		cout << "Для входя введите логин и пароль, или 0 для выхода:\n";
		cout << "Логин: ";
		cin >> login;
		if (cin.fail())
		{
			expCin();
			continue;
		}
		cout << "Пароль: ";
		cin >> password;
		if (cin.fail())
		{
			expCin();
			continue;
		}
		if (login == "0" || password == "0") break;

		string mes_cl ="l"+login+" "+password;
		me_=client(mes_cl);
		cout << endl<<me_<<endl;
		if (me_=="0")
		{
			me_ = "";
			cout << "Логин или пароль введены не верно\nПовторите ввод.\n";
			continue;
		}
		if (me_=="b")
		{
			me_ = "";
			cout << "Данный пользователь забанен\nПопробуйте другова пользователя.\n";
			continue;
		}
		if (me_=="-1")
		{
			me_ = "";
			cout << "Связь с сервером не удалась, попробуйте еще.\n";
			continue;
		}
	} while (me_ == "");
}

/*
	  --- ВСПОМОГАТЕЛЬНЫЙ МЕТОД метода LoginMenu ---
		registration - добавление новогопользователя в базу пользователей.
		В  даном  методе  пользователю  дается  возможность  добавить новую
	учетную запись, путем введения своих логина, пароля и имени.
		Производится  проверка  логина и имени на уникальность, при наличии
	повторов  в  базе  пользователей,  дается  возможность  повторить  ввод
	скорректировав повтор.
		По  результатам ввода вызывается дополнительный метод setUsersFile,
	для   добавления  нового  пользователя  в  базу  и  происходит  возврат
	в предыдущее меню.
*/
void Chat::registration()
{
	string login, password, name;
	while (true)
	{
		string tmp;
		system("clear");
		cout << "Для регистрации нового пользователя введите логин, пароль и имя пользователя.\n";
		cout << "Логин: ";
		cin >> login;
		if (cin.fail())
		{
			expCin();
			continue;
		}
		cout << "\nПароль: ";
		cin >> password;
		if (cin.fail())
		{
			expCin();
			continue;
		}
		cout << "\nИмя: ";
		cin >> name; 
		if (cin.fail())
		{
			expCin();
			continue;
		}
		getUsersFile();
		User user = User("false", login, password, name);
		users_.insert({ login, user });
		string mes_cl ="q";
		for (auto& user : users_)
		{
			mes_cl = mes_cl + user.second.getBan() + "\n" + user.second.getLogin() + "\n" + 
			user.second.getPassword() + "\n" + user.second.getName() + "\n";
		}
		tmp=client(mes_cl);
		return;
	}
}


/*
	  --- ВСПОМОГАТЕЛЬНЫЙ МЕТОД метода showChat ---
		getMessagePC_File  -  считывание  сообщений из файла messages.txt и
	внесение   их   в  вектор  message_,  при  этом  обрабатывается  ошибка
	открытия файла.
*/
void Chat::getMessage_File()
{
	int pos;
	message_.clear();
	string delTMP;
	string timeTMP;
	string fromTMP;
	string txtTMP;
	while (true)
	{
		string tmp = client("m");
		string inf=tmp;
		if (inf=="-1")
		{
			cout << "Ошибка получения данных о переписке, повторите попытку."<<endl;
			pauses();
			continue;
		}
		while (inf!="")
		{
			delTMP.clear();
			timeTMP.clear();
			fromTMP.clear();
			txtTMP.clear();
			delTMP.append(inf, 0, inf.find("\n"));
			inf.erase(0,inf.find("\n")+1);
			if (inf=="") break;
			timeTMP.append(inf, 0, inf.find("\n"));
			inf.erase(0,inf.find("\n")+1);
			if (inf=="") break;
			fromTMP.append(inf, 0, inf.find("\n"));
			inf.erase(0,inf.find("\n")+1);
			if (inf=="") break;
			if (inf.find("\n")!= -1) 
			{
				txtTMP.append(inf, 0, inf.find("\n"));
				inf.erase(0,inf.find("\n")+1);
			}else 
			{
				txtTMP = inf;
				inf.clear();
			}
			Message message = Message(delTMP, timeTMP, fromTMP, txtTMP);
			message_.push_back(message);
		}
		break;
	}
}

/*
	  --- ВСПОМОГАТЕЛЬНЫЙ МЕТОД метода showChat ---
		chat  -  вывод  на экран переписки в виде таблицы изходя из вектора
	message_ с его обновлением.
*/
void Chat::chat()
{
	getUsersFile();
	string from;
	cout << "Общий чат: " << "\n-----------------------------------------------------------------------\n";
	for (auto& mess : message_)
	{
		me_ == mess.getFrom() ? from = "(Я)" : from = mess.getFrom();
		if (me_ != "") if (users_.find(getNameToLogin(me_))->second.getBan() == "true")from += " (Забанен)";
		if (!mess.getTime().empty()) mess.getDelete() == "false" ? cout << mess.getTime() << "\t" << from << "\t" << mess.getText() << endl :
			cout << mess.getTime() << "\t" << from << "\t" << "Сообщение удалено модератором" << endl;
	}
	cout << "-----------------------------------------------------------------------" << endl;
}


/*
	  --- ВСПОМОГАТЕЛЬНЫЙ МЕТОД ---
		expCin - метод отработки ошибки при испльзовании оператора cin.
		В  даном  методе  производится  очистка буфера ввода, игнорирование
	ошибок и выводится сообщение пользователю сообщения об ошибке.
*/
void Chat::expCin()
{
	cin.clear();
	cin.ignore(32767, '\n');
	cout << "Ошибка ввода, повторите попытку.\n";
	cin.get();
};

//проверка логина на повторы
bool Chat::getUserByLogin(const string& login)const
{
	for (int i = 0; i < users_.size(); ++i);

	for (auto it = users_.begin(); it != users_.end(); ++it)
	{
		if (it->first == login)return true;
	}
	return false;
}

//проверка имени на повторы.
bool Chat::getUserByName(const string& name)const
{
	for (auto it = users_.begin(); it != users_.end(); ++it)
	{
		if (it->second.getName() == name)return true;
	}
	return false;
}

string Chat::getNameToLogin(string name)
{
	for (auto it = users_.begin(); it != users_.end(); ++it)
	{
		if (it->second.getName() == name)return it->first;
	}
	return "";

}

void Chat::pauses()
{
	char a;
	cin.clear();
	cin.ignore(32767, '\n');
	cout << endl << "Для продолжени нажмите любую клавишу.";
	cout << cin.get();//pause
	cin.clear();
	cin.ignore(32767, '\n');
}
