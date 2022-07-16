#include "Condition.h"

using namespace yazi::thread;

Condition::Condition()
{
    pthread_cond_init(&m_cond, NULL);
}

Condition::~Condition()
{
    pthread_cond_destroy(&m_cond);
}

int Condition::wait(Mutex* mutex)
{
    return pthread_cond_wait(&m_cond, &(mutex->m_mutex));
}

int Condition::signal()
{
    return pthread_cond_signal(&m_cond);
}

int Condition::broadcast()
{
    return pthread_cond_broadcast(&m_cond);
}


