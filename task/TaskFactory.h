#pragma once

#include "Socket.h"
using namespace yazi::socket;

#include "Task.h"
using namespace yazi::thread;

#include "WorkTask.h"
using namespace yazi::task;

namespace yazi {
namespace task {

class TaskFactory
{
public:
    static Task * create(Socket * socket)
    {
        return new WorkTask(socket);
    }
};

}}