#pragma once
#include <set>
#include "Thread.h"
#include "Mutex.h"
#include "AutoLock.h"
#include "Condition.h"
#include "Task.h"

namespace yazi {
namespace thread {


class ThreadPool
{
public:
    ThreadPool();
    ~ThreadPool();

    void create(int threads);

    Thread* get_idle_thread();

    void move_to_idle_list(Thread* thread);
    void move_to_busy_list(Thread* thread);

    int get_idle_thread_numbers();
    int get_busy_thread_numbers();

    void assign(Task* task);

private:
    int m_threads;
    Mutex m_mutex;

    std::set<Thread *> m_list_idle;
    std::set<Thread *> m_list_busy;

    Mutex m_mutex_idle;
    Mutex m_mutex_busy;

    Condition m_cond_idle;
    Condition m_cond_busy;
};

}}
