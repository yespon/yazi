#include "EchoTask.h"
using namespace yazi::task;

#include "Logger.h"
#include "Singleton.h"
using namespace yazi::utility;

#include "SocketHandler.h"
using namespace yazi::socket;


EchoTask::EchoTask(Socket * socket) : Task(socket)
{
}

EchoTask::~EchoTask()
{
}

void EchoTask::run()
{
    debug("echo task run");
    SocketHandler * handler = Singleton<SocketHandler>::instance();

    Socket * socket = static_cast<Socket *>(m_data);
    char buf[1024];
    memset(buf, 0, 1024);
    int len = socket->recv(buf, 1024);
    if (len > 0)
    {
        debug("recv msg len: %d msg data: %s", len, buf);
        socket->send(buf, len);
        handler->attach(socket);
    }
    else
    {
        debug("echo task socket closed by peer");
        handler->remove(socket);
    }
}

void EchoTask::destroy()
{
    debug("echo task destroy");
    delete this;
}
