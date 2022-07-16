#pragma once
#include <string>
using std::string;

#include <map>
using std::map;

namespace yazi {
namespace engine {

class Object
{
public:
    Object() {}
    virtual ~Object() {}
};


class Context
{
public:
    Context() {}
    ~Context() { clear(); }

    template <typename T>
    void set(const string &key, T value);

    template <typename T>
    T get(const string &key);

    template <typename T>
    T & ref(const string &key);

    void clear();

private:
    map<string, bool> m_bool;
    map<string, char> m_char;
    map<string, short> m_short;
    map<string, int> m_int;
    map<string, long> m_long;
    map<string, double> m_double;
    map<string, string> m_str;
    map<string, Object *> m_obj;
};

template<>
inline void Context::set<bool>(const string &key, bool value)
{
    m_bool[key] = value;
}

template<>
inline bool Context::get<bool>(const string &key)
{
    map<string, bool>::iterator it = m_bool.find(key);
    if (it != m_bool.end())
        return it->second;

    return false;
}

template <>
inline void Context::set<char>(const string &key, char value)
{
    m_char[key] = value;
}

template <>
inline char Context::get<char>(const string &key)
{
    map<string, char>::iterator it = m_char.find(key);
    if (it != m_char.end())
        return it->second;
    return 0;
}

template <>
inline void Context::set<unsigned char>(const string &key, unsigned char value)
{
    m_char[key] = value;
}

template <>
inline unsigned char Context::get<unsigned char>(const string &key)
{
    map<string, char>::iterator it = m_char.find(key);
    if (it != m_char.end())
        return static_cast<unsigned char>(it->second);
    return 0;
}

template <>
inline void Context::set<short>(const string &key, short value)
{
    m_short[key] = value;
}

template <>
inline short Context::get<short>(const string &key)
{
    map<string, short>::iterator it = m_short.find(key);
    if (it != m_short.end())
        return it->second;
    return 0;
}

template <>
inline void Context::set<unsigned short>(const string &key, unsigned short value)
{
    m_short[key] = value;
}

template <>
inline unsigned short Context::get<unsigned short>(const string &key)
{
    map<string, short>::iterator it = m_short.find(key);
    if (it != m_short.end())
        return static_cast<unsigned short>(it->second);
    return 0;
}

template <>
inline void Context::set<int>(const string &key, int value)
{
    m_int[key] = value;
}

template <>
inline int Context::get<int>(const string &key)
{
    map<string, int>::iterator it = m_int.find(key);
    if (it != m_int.end())
        return it->second;
    return 0;
}

template <>
inline void Context::set<unsigned int>(const string &key, unsigned int value)
{
    m_int[key] = value;
}

template <>
inline unsigned int Context::get<unsigned int>(const string &key)
{
    map<string, int>::iterator it = m_int.find(key);
    if (it != m_int.end())
        return static_cast<unsigned int>(it->second);
    return 0;
}

template <>
inline void Context::set<long>(const string &key, long value)
{
    m_long[key] = value;
}

template <>
inline long Context::get<long>(const string &key)
{
    map<string, long>::iterator it = m_long.find(key);
    if (it != m_long.end())
        return it->second;
    return 0;
}

template <>
inline void Context::set<unsigned long>(const string &key, unsigned long value)
{
    m_long[key] = value;
}

template <>
inline unsigned long Context::get<unsigned long>(const string &key)
{
    map<string, long>::iterator it = m_long.find(key);
    if (it != m_long.end())
        return static_cast<unsigned long>(it->second);
    return 0;
}

template <>
inline void Context::set<double>(const string &key, double value)
{
    m_double[key] = value;
}

template <>
inline double Context::get<double>(const string &key)
{
    map<string, double>::iterator it = m_double.find(key);
    if (it != m_double.end())
        return it->second;
    return 0;
}

template <>
inline void Context::set<const string &>(const string &key, const string &value)
{
    m_str[key] = value;
}

template <>
inline string & Context::get<string &>(const string &key)
{
    return m_str[key];
}

template <>
inline void Context::set<Object *>(const string &key, Object* value)
{
    m_obj[key] = value;
}

template <>
inline Object * Context::get<Object *>(const string &key)
{
    map<string, Object *>::iterator it = m_obj.find(key);
    if (it != m_obj.end())
        return it->second;
    return NULL;
}

template <>
inline string & Context::ref<string>(const string &key)
{
    return m_str[key];
}

template <typename T>
inline T & Context::ref(const string &key)
{
    map<string, Object *>::iterator it = m_obj.find(key);
    if (it != m_obj.end())
    {
        return *reinterpret_cast<T *>(it->second);
    }
    else
    {
        m_obj[key] = new T();
        return *reinterpret_cast<T *>(m_obj[key]);
    }
}

inline void Context::clear()
{
    m_bool.clear();
    m_char.clear();
    m_short.clear();
    m_int.clear();
    m_long.clear();
    m_double.clear();
    m_str.clear();
    for (map<string, Object *>::iterator it = m_obj.begin(); it != m_obj.end(); ++it)
    {
        delete it->second;
        it->second = NULL;
    }
    m_obj.clear();
}

}}
