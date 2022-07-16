#include <iostream>
using namespace std;

#include "System.h"
#include "IniFile.h"
#include "Singleton.h"
using namespace yazi::utility;

#include "Server.h"
using namespace yazi::server;


int main()
{
    System * sys = Singleton<System>::instance();
    sys->init();

    IniFile * ini = Singleton<IniFile>::instance();
    const string & ip = (*ini)["server"]["ip"];
    int port = (*ini)["server"]["port"];
    int threads = (*ini)["server"]["threads"];
    int max_conn = (*ini)["server"]["max_conn"];

    Server * server = Singleton<Server>::instance();
    server->listen(ip, port);
    server->set_threads(threads);
    server->set_connects(max_conn);
    server->start();

    return 0;
}
