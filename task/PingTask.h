#pragma once

#include "Task.h"
using namespace yazi::thread;

#include "Socket.h"
using namespace yazi::socket;

namespace yazi {
namespace task {

class PingTask : public Task
{
public:
    PingTask(Socket * socket);
    virtual ~PingTask();

    virtual void run();

    virtual void destroy();
};

}}
