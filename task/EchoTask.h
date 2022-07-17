#pragma once

#include "Task.h"
using namespace yazi::thread;

#include "Socket.h"
using namespace yazi::socket;

namespace yazi {
namespace task {

class EchoTask : public Task
{
public:
    EchoTask(Socket * socket);
    virtual ~EchoTask();

    virtual void run();

    virtual void destroy();
};

}}
