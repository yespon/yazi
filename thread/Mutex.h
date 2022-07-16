#pragma once
#include <pthread.h>

namespace yazi {
namespace thread {

class Mutex
{
    friend class Condition;

public:
    Mutex();
    ~Mutex();

    // lock
    int lock();

    // try to lock
    int try_lock();

    // unlock
    int unlock();

private:
    pthread_mutex_t     m_mutex;
};

}}
