#include "Server.h"
#include <filesystem>
#include <locale>
#include <iostream>
using namespace std;



int main()
{
	locale ru("ru_RU.utf8");
   	auto permissions=std::filesystem::perms::group_all | std::filesystem::perms::others_all;
	std::filesystem::permissions("./Messages.txt", permissions, std::filesystem::perm_options::remove);
   	std::filesystem::permissions("./Users.txt", permissions, std::filesystem::perm_options::remove);
	std::filesystem::permissions("./MessagesPC.txt", permissions, std::filesystem::perm_options::remove);

	Server server;
	server.miServer();
	return 0;
}
