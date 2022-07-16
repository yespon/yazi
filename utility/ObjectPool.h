#pragma once
#include <list>

#include "Mutex.h"
#include "AutoLock.h"

using namespace yazi::thread;

namespace yazi {
namespace utility {

template <typename T>
class ObjectPool
{
public:
    ObjectPool();
    ~ObjectPool();

    void init(int max);
    T * allocate();
    void release(T * p);

private:
    std::list<T *> m_pool;
    Mutex m_mutex;
};

template <typename T>
ObjectPool<T>::ObjectPool()
{
}

template <typename T>
ObjectPool<T>::~ObjectPool()
{
    AutoLock lock(&m_mutex);
    for (typename std::list<T *>::iterator it = m_pool.begin(); it != m_pool.end(); it++)
    {
        if ((*it) != NULL)
        {
            delete (*it);
        }
    }
    m_pool.clear();
}

template <typename T>
void ObjectPool<T>::init(int max)
{
    AutoLock lock(&m_mutex);
    for (int i = 0; i < max; i++)
    {
        T * p = new T();
        m_pool.push_back(p);
    }
}

template <typename T>
T * ObjectPool<T>::allocate()
{
    AutoLock lock(&m_mutex);
    if (m_pool.size() == 0)
    {
        return NULL;
    }
    T * p = m_pool.front();
    m_pool.pop_front();
    return p;
}

template <typename T>
void ObjectPool<T>::release(T * p)
{
    AutoLock lock(&m_mutex);
    m_pool.push_back(p);
}

}}
