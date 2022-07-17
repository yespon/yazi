#include "WorkTask.h"

#include "Logger.h"
#include "Singleton.h"
using namespace yazi::utility;

#include "SocketHandler.h"
using namespace yazi::socket;

#include "Workflow.h"
using namespace yazi::engine;

using namespace yazi::task;

#include <sstream>
using std::ostringstream;


WorkTask::WorkTask(Socket * socket) : Task(socket)
{
}

WorkTask::~WorkTask()
{
}

void WorkTask::run()
{
    debug("work task run");
    SocketHandler * handler = Singleton<SocketHandler>::instance();

    Socket * socket = static_cast<Socket *>(m_data);

    MsgHead msg_head;
    memset(&msg_head, 0, sizeof(msg_head));
    int len = socket->recv((char *)(&msg_head), sizeof(msg_head));
    if (len == 0)
    {
        error("socket closed by peer");
        handler->remove(socket);
        return;
    }
    if (len == -1 && errno == EAGAIN)
    {
        error("socket recv len: %d, error msg: EAGAIN errno: %d", len, errno);
        handler->attach(socket);
        return;
    }
    if (len == -1 && errno == EWOULDBLOCK)
    {
        error("socket recv len: %d, error msg: EWOULDBLOCK errno: %d", len, errno);
        handler->attach(socket);
        return;
    }
    if (len == -1 && errno == EINTR)
    {
        error("socket recv len: %d, error msg: EINTR errno: %d", len, errno);
        handler->attach(socket);
        return;
    }
    if (len != sizeof(msg_head))
    {
        error("recv msg head error length: %d, errno: %d", len, errno);
        handler->remove(socket);
        return;
    }
    info("recv msg head len: %d, flag: %s, cmd: %d, body len: %d", len, msg_head.flag, msg_head.cmd, msg_head.len);
    if (strncmp(msg_head.flag, "work", 4) != 0)
    {
        error("msg head flag error");
        handler->remove(socket);
        return;
    }

    if (msg_head.len >= uint32_t(recv_buff_size))
    {
        error("recv msg body len: %d, large than recv_buff_size: %d", msg_head.len, recv_buff_size);
        handler->remove(socket);
        return;
    }

    char buf[recv_buff_size];
    memset(buf, 0, recv_buff_size);
    len = socket->recv(buf, msg_head.len);
    if (len == -1 && errno == EAGAIN)
    {
        error("socket recv len: %d, error msg: EAGAIN errno: %d", len, errno);
        handler->remove(socket);
        return;
    }
    if (len == -1 && errno == EWOULDBLOCK)
    {
        error("socket recv len: %d, error msg: EWOULDBLOCK errno: %d", len, errno);
        handler->remove(socket);
        return;
    }
    if (len == -1 && errno == EINTR)
    {
        error("socket recv len: %d, error msg: EINTR errno: %d", len, errno);
        handler->remove(socket);
        return;
    }
    if (len != (int)(msg_head.len))
    {
        error("recv msg body error length: %d, body: %s, errno: %d", len, buf, errno);
        handler->remove(socket);
        return;
    }

    info("recv msg body len: %d, msg data: %s", len, buf);

    Workflow * workflow = Singleton<Workflow>::instance();

    ostringstream os;
    os << (int)(msg_head.cmd);
    const string work = os.str();
    const string input = buf;
    string output;

    workflow->run(work, input, output);

    socket->send(output.c_str(), output.length());
    handler->attach(socket);
}

void WorkTask::destroy()
{
    debug("work job destory");
    delete this;
}
