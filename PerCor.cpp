﻿#include <iostream>
#include <ctime>
#include <fstream>
#include "PerCor.h"
//#include <Windows.h>

/*
	  --- ОСНОВНОЙ МЕТОД класса личной переписки PerCop ---
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
void PerCor::loginMenuPC()
{
	string operation;
	getUsersFile();
	while (true)
	{
		system("clear");
		cout << "1. Вход.\n2. Регистрация.\n0. Выход в предыдущее меню.\n";
		cin >> operation;
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
		                cin.clear();
                		cin.ignore(32767,'\n');
		                cout << cin.get();//pause
			}
			continue;
		}
		if (operation == "0")
		{
			break;
		}
		cout << "Выбор не пределен. Повторите попытку.\n";
		cin.clear();
		cin.ignore(32767,'\n');
		cout << cin.get();//pause
		continue;
	}
}

/*
	  --- ОСНОВНОЙ МЕТОД класса личной переписки PerCop ---
		userMenuPC - выбор опонента по переписке.
		Выбор  может  быть  произведен  из уже начатых переписок или начата
	новая переписка.

*/
void PerCor::userMenuPC()
{
	system("clear");
	/*
	Далее происходт загрузка личных переписок из файла messagesPC.txt,
	чтобы определить какие переписки уже открыты.
	*/
	getMessagePC_File();
	/*
	далее с помощью метода perCorParty, заполняется набор perCorParty_(список пользователей, с которыми открыта переписка),
	чтобы в выводимой таблице пользователей мы могли указать с кем переписка уже ведется.
	*/
	perCorParty();
	getUsersFile();
	cout << "Привет, " << me_ << ".\nУ вас имеется переписка с " << perCorParty_.size() << " участник(ов, ом, ами).\n";
	unsigned int tmp = 1;
	for (auto const name : perCorParty_)
	{
		cout << tmp << ". " << name;
		if (users_.find(getNameToLogin(name))->second.getBan() == "true")cout << " (Забанен)";
		cout << endl;
		tmp++;
	}
	while (true)
	{
		cout << "Выберите с кем продолжить переписку, " << (perCorParty_.size())+1 << " для добавления нового участника, 0 - выход\n";
		cin >> tmp;
		if (cin.fail())
		{
			expCin();
			continue;
		}
		if (tmp > (perCorParty_.size())+1)
		{
			cout << "Ошибка ввода.\nВведите от 0 до " << (perCorParty_.size()) << endl;
	                cin.clear();
	                cin.ignore(32767,'\n');
	                cout << cin.get();//pause
			continue;
		}
		if (tmp == 0) break;
		if (perCorParty_.size() > 0) if (tmp <= perCorParty_.size()) {
			int tmp1 = 1;
			for (string name : perCorParty_)
			{
				if (tmp==tmp1)
				{
					party_ = name;	
					return;
				}
				tmp1++;
			}
			break;
		}
		if (tmp == (perCorParty_.size())+1)
		{
			addParty();
			break;
		}
	}

}

/*
	  --- ОСНОВНОЙ МЕТОД класса личной переписки PerCop ---
		showPC   -   вывод   на  экран  переписки  с  выбранным  оппонентом
	с возможностью продолжения переписки.
		В  данном  методе  предоставлен  бесконечный цикл, который  выводит
	переписку  с  оппонентом  и  дает возможность написать новое сообщение,
	обновить переписку,	чтобы проверить новые сообщения от оппонента.
*/
void PerCor::showPC()
{
	string txt;
	/*
	Применение бесконечного цикла обеспечивает непрерывнсть работы
	и возможность обновлять переписку.
	*/
	while (true)
	{
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
		cin.clear();
		if (!txt.empty())
		{
			MessagePC messagePC = MessagePC("false", time(), me_, party_, txt);
			messagePC_.push_back(messagePC);
			string mes_cl ="e";
			for (auto& mess : messagePC_)
			{
				mes_cl = mes_cl + mess.getDelete() + "\n" + mess.getTime() + "\n" + 
				mess.getFrom() + "\n" + mess.getTo()+ "\n" + mess.getText() + "\n";
			}
			string tmp=client(mes_cl);
		}
	}
}

/*
	  --- ОСНОВНОЙ МЕТОД класса личной переписки PerCop ---
		loginAdminPC - вход в личную переписку админа из класса Admin.
		В  данном  методе  определяется значение указателя currentUser_ как
	admin, от лица которого происходит общение.
		База   пользователей   загружается   из   файла   User.txt.
*/
void PerCor::loginAdminPC()
{
	string login = "admin";
	getUsersFile();
	me_ = "admin";
}




/*
	  --- ВСПОМОГАТЕЛЬНЫЙ МЕТОД метода loginMenuPC ---
		getUsersFile - загрузка пользователей из файла User.txt.
		В  даном  методе  мы  заполняем  вектор  User,  данными  считанными
	из  файла  User.txt.  При  этом  происходит  проверка на доступ к файлу
	в случае если он открыт другим пользователем.
*/
void PerCor::getUsersFile()
{
	users_.clear();
	string banTMP;
	string loginTMP;
	string passwordTMP;
	string nameTMP;
	while (true)
	{
		system("clear");
		/*
		повторить операцию, чтобы обеспечить обязательность загрузки базы пользователей.
		*/
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
	  --- ВСПОМОГАТЕЛЬНЫЙ МЕТОД метода loginMenuPC ---
		login - вход в прорамму по логину и паролю.
		В  даном  методе  пользователю дается возможность ввести свой логин
	и пароль. Производится их проверка, при наличии такой комбинации в базе
	пользователей     выбирается    текущий    пользователь    currentUser_
	и  присваивается  переменной  me_  имя текущео пользователя. если такой
	логин   отсутсвует   или  пароль  не  соответствует  введеному  логину,
	выводится ошибка, и предложение повториь ввод.
*/
void PerCor::login() {
	string login, password;
	do
	{
		system("clear");
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
		me_ = users_.find(login)->second.getName();
		if (password != users_.find(login)->second.getPassword())
		{
			me_ = "";
			cout << "Логин или пароль введены не верно\nПовторите ввод.\n";
			cin.clear();
			cin.ignore(32767, '\n');
			cout << cin.get();
			continue;
		}
		if (users_.find(login)->second.getBan() == "true")
		{
			me_ = "";
			cout << "Данный пользователь забанен\nПопробуйте другова пользователя.\n";
			cin.clear();
			cin.ignore(32767, '\n');
			cout << cin.get();
			continue;
		}
	} while (me_=="");
}

/*
	  --- ВСПОМОГАТЕЛЬНЫЙ МЕТОД метода loginMenuPC ---
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
void PerCor::registration()
{
	string login, password, name;
	while (true)
	{
		system("clear");
		cout << "Для регистрации нового пользователя введите логин, пароль и имя пользователя.\n";
		cout << "Логин: ";
		cin >> login;
		if (cin.fail())
		{
			expCin();
			continue;
		}
		if (getUserByLogin(login) || login == "Admin" || login == "0")throw ExpLogin();
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
		if (getUserByName(name) || name == "0") throw ExpName();
		getUsersFile();
		User user = User("false", login, password, name);
		users_.insert({ login, user });
		string mes_cl ="q";
		for (auto& user : users_)
		{
			mes_cl = mes_cl + user.second.getBan() + "\n" + user.second.getLogin() + "\n" + 
			user.second.getPassword() + "\n" + user.second.getName() + "\n";
		}
		string tmp = client(mes_cl);
		return;
	}
}


/*
	  --- ВСПОМОГАТЕЛЬНЫЙ МЕТОД метода userMenuPC ---
		getMessagePC_File  - считывание сообщений из файла messagesPC.txt и
	внесение  их  в  вектор  messagePC_,  при  этом  обрабатывается  ошибка
	открытия файла.
*/
void PerCor::getMessagePC_File()
{
	messagePC_.clear();
	string delTMP;
	string timeTMP;
	string fromTMP;
	string toTMP;
	string txtTMP;
	while (true)
	{
		string tmp = client("p");
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
			toTMP.clear();
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
			toTMP.append(inf, 0, inf.find("\n"));
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
			MessagePC messagePC = MessagePC(delTMP, timeTMP, fromTMP, toTMP, txtTMP);
			messagePC_.push_back(messagePC);
		}
		break;
	}
}

/*
	  --- ВСПОМОГАТЕЛЬНЫЙ МЕТОД метода userMenuPC ---
		perCorParty    -   составление   вектора   perCorParty_   (перечень
	пользователей  с  которыми  уже  начата  переписка),  на основе вектора
	messagePC_ (персональная переписка).
*/
void PerCor::perCorParty()
{
	string tmp_name;
	bool tmp;
	for (auto& MessagesPC : messagePC_)
	{

		tmp = false;
		if (MessagesPC.getFrom() == me_)tmp_name = MessagesPC.getTo();
		if (MessagesPC.getTo() == me_) tmp_name = MessagesPC.getFrom();
		for (auto& name : perCorParty_)
		{
			if (name == tmp_name)
			{
				tmp = true;
				break;
			}
		}
		if (tmp == false) if(tmp_name!="") perCorParty_.insert(tmp_name);
	}
}

/*
	  --- ВСПОМОГАТЕЛЬНЫЙ МЕТОД метода userMenuPC ---
		addParty - добавление в вектор perCorParty_ (перечень пользователей
	скоторыми   уже   начата   переписка),   нового   оппонента.  При  этом
	проверяется, открыта или нет, переписка с данным оппонентом.
*/
void PerCor::addParty()
{
	unsigned int tmp;
	while (true)
	{
		tmp = 1;
		set<string> names;
		getUsersFile();
		cout << "Имеются следующие участники:\n";
		for (map<string, User>::iterator it = users_.begin(); it != users_.end(); ++it)
		{
			cout << tmp << ". " << it->second.getName();
			if (it->second.getBan() == "true")cout << " (Забанен)";
			for (auto& name : perCorParty_)
			{
				if (name == it->second.getName())
				{
					cout << " (переписка открыта)";
					break;
				}
			}
			if (it->second.getName() == me_)
			{
				cout << " (я)";
			}
			cout << endl;
			names.insert(it->second.getName());
			tmp++;
		}
		cout << "С кем начать переписку? Выберите от 1 до " << (users_.size()) << ", 0 - выход.\n";
		cin >> tmp;
		if (cin.fail())
		{
			expCin();
			continue;
		}
		if (tmp > users_.size())
		{
			cout << "Некорректный ввод, повторите попытку.\n";
			cin.clear();
			cin.ignore(32767, '\n');
			cout << cin.get();//pause
			continue;
		}
		if (tmp == 0)
		{
			party_="";
			break;
		}
		int a = 1;
		for (auto name : names)
		{
			if (a == tmp) 
			{
				if (PerCor::expParty(name))
				{
					party_ = name;
					return;
				}

			}
			a++;
		}
	}
}

/*
	  --- ВСПОМОГАТЕЛЬНЫЙ МЕТОД метода addParty ---
		expParty - проверка не открыта ли переписка с выбранным опонентом и
	не собирается ли пользователь говорить сам с собой.
*/
bool PerCor::expParty(const string name)
{
	for (auto &names : perCorParty_)
	{
		if (name == names)
		{
			cout << "С этим пользователем переписка уже открыта.\nПовторите выбор\n";
			cin.clear();
			cin.ignore();
			cout << cin.get();//pause
			return false;
		}
	}
	if (name == me_)
	{
		srand(static_cast<unsigned int>(time(0)));
		int tmp = (rand() % 3);
		switch (tmp)
		{
		case 0:
			cout << "Как ты собираешься говорить сам с собой?\n";
			cin.clear();
			cin.ignore(32767,'\n');
			cout << cin.get();//pause
			return false;
		case 1:
			cout << "Если вы говорите с зеркалом, обратитесь к психиатору\n";
			cin.clear();
			cin.ignore(32767,'\n');
			cout << cin.get();//pause
			return false;
		case 2:
			cout << "С самим собой переписка не доступна\n";
			cin.clear();
			cin.ignore(32767, '\n');
			cout << cin.get();//pause
			return false;
		default:
			cout << "Ошибка ввода, повторите попытку\n";
			cin.clear();
			cin.ignore(32767, '\n');
			cout <<cin.get();//pause
			return false;
		}
	}
	return true;
}

/*
	  --- ВСПОМОГАТЕЛЬНЫЙ МЕТОД метода showPC ---
		chat  -  вывод  на экран переписки с выбранным пользователем в виде
	таблицы	изходя из вектора messagePC_.
*/
void PerCor::chat()
{
	system("clear");
	string from, to;
	getMessagePC_File();
	cout << "Переписка с " << party_ << "\n-----------------------------------------------------------------------\n";
	for (auto& mess : messagePC_)
	{
		/*
		Происходит выборка сообщений, для отображения только тех сообщений,
		которые относятся к данной переписке (пользователя с оппонентом)
		*/
		if ((me_ == mess.getFrom() && party_ == mess.getTo()) || (me_ == mess.getTo() && party_ == mess.getFrom()))
		{
			if (me_ == mess.getFrom())
			{
				from = "(Я)";
				to = mess.getTo();
			}
			else
			{
				from = mess.getFrom();
				to = "(Мне)";
			}
			mess.getDelete() == "false" ? cout << mess.getTime() << "\t" << from << " -> " << to << "\t" << mess.getText() << endl :
				cout << mess.getTime() << "\t" << from << " -> " << to << "\t" << "Сообщение удалено модератором" << endl;
		}
	}
	cout << "-----------------------------------------------------------------------" << endl;
}


/*
	  --- ВСПОМОГАТЕЛЬНЫЙ МЕТОД ---
		expCin - метод отработки ошибки при испльзовании оператора cin.
		В  даном  методе  производится  очистка буфера ввода, игнорирование
	ошибок и выводится сообщение пользователю сообщения об ошибке.
*/
void PerCor::expCin()
{
	cin.clear();
	cin.ignore(32767, '\n');
	cout << "Ошибка ввода, повторите попытку.\n";
	cout << cin.get();//pause
	cin.clear();
	cin.ignore(32767, '\n');
};


//проверка логина на повторы
bool PerCor::getUserByLogin(const string& login)const
{
	for (int i = 0; i < users_.size(); ++i);

	for (auto it = users_.begin(); it != users_.end(); ++it)
	{
		if (it->first == login)return true;
	}
	return false;
}

//проверка имени на повторы.
bool PerCor::getUserByName(const string& name)const
{
	for (auto it = users_.begin(); it != users_.end(); ++it)
	{
		if (it->second.getName() == name)return true;
	}
	return false;
}

string PerCor::getNameToLogin(string name)
{
	for (auto it = users_.begin(); it != users_.end(); ++it)
	{
		if (it->second.getName() == name)return it->first;
	}
	return "";

}

void PerCor::pauses()
{
	char a;
	cin.clear();
	cin.ignore(32767, '\n');
	cout << endl << "Для продолжени нажмите любую клавишу.";
	cout << cin.get();//pause
	cin.clear();
	cin.ignore(32767, '\n');
}



