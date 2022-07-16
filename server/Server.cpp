#include "Server.h"
using namespace yazi::server;

#include "Singleton.h"
using namespace yazi::utility;

#include "TaskDispatcher.h"
using namespace yazi::thread;

#include "SocketHandler.h"
using namespace yazi::socket;

Server::Server() : m_ip(""), m_port(0), m_threads(1024), m_connects(1024)
{
}

Server::~Server()
{
}

void Server::listen(const string & ip, int port)
{
    m_ip = ip;
    m_port = port;
}

void Server::start()
{
    // initialize the thread pool and task queue
    TaskDispatcher * dispatcher = Singleton<TaskDispatcher>::instance();
    dispatcher->init(m_threads);

    // initialize the socket handler
    SocketHandler * socket_handler = Singleton<SocketHandler>::instance();
    socket_handler->listen(m_ip, m_port);
    socket_handler->handle(m_connects);
}

void Server::set_threads(int threads)
{
    m_threads = threads;
}

void Server::set_connects(int connects)
{
    m_connects = connects;
}
