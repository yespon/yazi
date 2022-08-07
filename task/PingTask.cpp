#include "PingTask.h"
using namespace yazi::task;

#include "Logger.h"
#include "Singleton.h"
using namespace yazi::utility;

#include "SocketHandler.h"
using namespace yazi::socket;
#include <cstdio>
#include <unistd.h>


PingTask::PingTask(Socket * socket) : Task(socket)
{
}

PingTask::~PingTask()
{
}

void PingTask::run()
{
    debug("Ping task run");
    SocketHandler * handler = Singleton<SocketHandler>::instance();

    Socket * socket = static_cast<Socket *>(m_data);
    char buf[8192];
    memset(buf, 0, 8192);
    int len = socket->recv(buf, 8192);
    if (len > 0)
    {
        debug("recv msg len: %d", len);
        socket->send("+OK\r\n", 5);
        handler->attach(socket);
    }
    else
    {
        debug("Ping task socket closed by peer");
        handler->remove(socket);
    }
}

void PingTask::destroy()
{
    debug("Ping task destroy");
    delete this;
}
