#pragma once

#include <pthread.h>
#include <signal.h>
#include <list>
#include "Thread.h"
#include "ThreadPool.h"
#include "Task.h"


namespace yazi {
namespace thread {

class TaskDispatcher : public Thread
{
public:
    TaskDispatcher();
    ~TaskDispatcher();

    void init(int threads);
    void assign(Task* task);
    void handle(Task* task);
    virtual void run();

protected:
    std::list<Task *> m_tasks;
};

}}
