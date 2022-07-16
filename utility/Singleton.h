#pragma once

namespace yazi {
namespace utility {

template <typename T>
class Singleton
{
public:
    static T * instance()
    {
        static T * instance = NULL;
        if (instance == NULL)
            instance = new T();
        return instance;
    }

protected:
    Singleton() {}
    Singleton(const Singleton<T> &);
    Singleton<T> & operator = (const Singleton<T> &);
    ~Singleton() {}
};

}}
