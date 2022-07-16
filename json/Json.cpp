#include <iostream>
#include <sstream>
using std::istringstream;
using std::ostringstream;

#include <stdexcept>

#include "Json.h"
#include "Parser.h"
using namespace yazi::json;

Json::Json() : m_type(json_null), m_int(0), m_double(0.0), m_bool(false)
{
}

Json::Json(Type type) : m_type(type), m_int(0), m_double(0.0), m_bool(false)
{
}

Json::Json(bool value)
{
    m_type = json_bool;
    m_bool = value;
}

Json::Json(int value)
{
    m_type = json_int;
    m_int = value;
}

Json::Json(double value)
{
    m_type = json_double;
    m_double = value;
}

Json::Json(const char * value)
{
    m_type = json_string;
    m_string = value;
}

Json::Json(const string & value)
{
    m_type = json_string;
    m_string = value;
}

Json::Json(const Json & other)
{
    copy(other);
}

Json::~Json()
{
    clear();
}

Json::Type Json::type() const
{
    return m_type;
}

bool Json::isNull() const
{
    return (type() == json_null);
}

bool Json::isBool() const
{
    return (type() == json_bool);
}

bool Json::isInt() const
{
    return (type() == json_int);
}

bool Json::isDouble() const
{
    return (type() == json_double);
}

bool Json::isString() const
{
    return (type() == json_string);
}

bool Json::isArray() const
{
    return (type() == json_array);
}

bool Json::isObject() const
{
    return (type() == json_object);
}

bool Json::asBool() const
{
    switch (type())
    {
        case json_bool:
            return m_bool;
        default:
            break;
    }
    throw std::logic_error("function Json::asBool value type error");
}

int Json::asInt() const
{
    switch (type())
    {
        case json_int:
            return m_int;
        default:
            break;
    }
    throw std::logic_error("function Json::asInt value type error");
}

double Json::asDouble() const
{
    switch (type())
    {
        case json_double:
            return m_double;
        default:
            break;
    }
    throw std::logic_error("function Json::asDouble value type error");
}

string Json::asString() const
{
    switch (type())
    {
        case json_string:
            return m_string;
        default:
            break;
    }
    throw std::logic_error("function Json::asString value type error");
}

void Json::copy(const Json & other)
{
    clear();

    m_type = other.type();

    switch (other.type())
    {
        case json_null:
            break;
        case json_int:
            m_int = other.m_int;
            break;
        case json_double:
            m_double = other.m_double;
            break;
        case json_bool:
            m_bool = other.m_bool;
            break;    
        case json_string:
            m_string = other.m_string;
            break;
        case json_array:
            m_array = other.m_array;
            break;
        case json_object:
            m_object = other.m_object;
            break;
        default:
            break;
    }
}

int Json::size() const
{
    switch (type())
    {
        case json_array:
            return m_array.size();
        case json_object:
            return m_object.size();
        default:
            break;
    }
    throw std::logic_error("function Json::size value type error");
}

bool Json::empty() const
{
    switch (type())
    {
        case json_null:
            return true;
        case json_array:
            return m_array.empty();
        case json_object:
            return m_object.empty();
        default:
            break;
    }
    throw std::logic_error("function Json::empty value type error");
}

void Json::clear()
{
    m_int = 0;
    m_double = 0.0;
    m_bool = false;
    m_string.clear();
    m_array.clear();
    m_object.clear();
}

bool Json::remove(int index)
{
    if (type() != json_array)
    {
        return false;
    }
    int size = m_array.size();
    if (index < 0 || index >= size)
    {
        return false;
    }
    m_array.erase(m_array.begin() + index);
    return true;
}

bool Json::remove(const char * key)
{
    string name = key;
    return remove(name);
}

bool Json::remove(const string & key)
{
    if (type() != json_object)
    {
        return false;
    }
    std::map<string, Json>::iterator it = m_object.find(key);
    if (it == m_object.end())
    {
        return false;
    }
    m_object.erase(it);
    return true;
}

bool Json::has(int index)
{
    if (type() != json_array)
    {
        return false;
    }
    int size = m_array.size();
    return (index >= 0 && index < size);
}

bool Json::has(const char * key)
{
    string name(key);
    return has(name);
}

bool Json::has(const string & key)
{
    if (type() != json_object)
    {
        return false;
    }
    std::map<string, Json>::const_iterator it = m_object.find(key);
    return (it != m_object.end());
}

const Json & Json::get(int index) const
{
    if (type() != json_array)
    {
        throw std::logic_error("function Json::get [int] requires array value");
    }
    int size = m_array.size();
    if (index >= 0 && index < size)
    {
        return m_array.at(index);
    }
    return null();
}

const Json & Json::get(const char * key) const
{
    const string name = key;
    return get(name);
}

const Json & Json::get(const string & key) const
{
    if (type() != json_object)
    {
        throw std::logic_error("function Json::get [const string &] requires object value");
    }
    std::map<string, Json>::const_iterator it = m_object.find(key);
    if (it != m_object.end())
    {
        return it->second;
    }
    return null();
}

void Json::set(const Json & other)
{
    copy(other);
}

void Json::set(bool value)
{
    clear();
    m_type = json_bool;
    m_bool = value;
}

void Json::set(int value)
{
    clear();
    m_type = json_int;
    m_int = value;
}

void Json::set(double value)
{
    clear();
    m_type = json_double;
    m_double = value;
}

void Json::set(const string & value)
{
    clear();
    m_type = json_string;
    m_string = value;
}

void Json::set(const char * value)
{
    clear();
    m_type = json_string;
    m_string = value;
}

Json & Json::append(const Json & value)
{
    if (type() == json_null)
    {
        m_type = json_array;
    }
    if (type() != json_array)
    {
        throw std::logic_error("function Json::append requires array value");
    }
    m_array.push_back(value);
    return (*this);
}

Json & Json::operator = (const Json& other)
{
    copy(other);
    return *this;
}

Json & Json::operator = (bool value)
{
    Json other(value);
    copy(other);
    return *this;
}

Json & Json::operator = (int value)
{
    Json other(value);
    copy(other);
    return *this;
}

Json & Json::operator = (double value)
{
    Json other(value);
    copy(other);
    return *this;
}

Json & Json::operator = (const string & value)
{
    Json other(value);
    copy(other);
    return *this;
}

Json & Json::operator = (const char * value)
{
    Json other(value);
    copy(other);
    return *this;
}

bool Json::operator == (const Json & other)
{
    if (type() != other.type())
    {
        return false;
    }
    switch (type())
    {
        case json_null:
            return true;
        case json_bool:
            return (m_bool == other.m_bool);
        case json_int:
            return (m_int == other.m_int);
        case json_double:
            return (m_double == other.m_double);
        case json_string:
            return (m_string == other.m_string);
        default:
            break;
    }
    return false;
}

bool Json::operator == (bool value)
{
    Json other = value;
    return (*this == other);
}

bool Json::operator == (int value)
{
    Json other = value;
    return (*this == other);
}

bool Json::operator == (double value)
{
    Json other = value;
    return (*this == other);
}

bool Json::operator == (const string & value)
{
    Json other = value;
    return (*this == other);
}

bool Json::operator == (const char * value)
{
    Json other = value;
    return (*this == other);
}

bool Json::operator != (const Json & other)
{
    return !(*this == other);
}


bool Json::operator != (bool value)
{
    Json other = value;
    return (*this != other);
}

bool Json::operator != (int value)
{
    Json other = value;
    return (*this != other);
}

bool Json::operator != (double value)
{
    Json other = value;
    return (*this != other);
}

bool Json::operator != (const string & value)
{
    Json other = value;
    return (*this != other);
}

bool Json::operator != (const char * value)
{
    Json other = string(value);
    return (*this != other);
}

Json & Json::operator [] (int index)
{
    if (type() == json_null)
    {
        m_type = json_array;
    }
    if (type() != json_array)
    {
        throw std::logic_error("function Json::operator [int] requires array value");
    }
    int size = m_array.size();
    if (index < 0)
    {
        throw std::logic_error("function Json::operator [int] index less 0");
    }
    if (index > 0 && index < size)
    {
        return m_array.at(index);
    }
    if (index >= size)
    {
        for (int i = size; i <= index; i++)
        {
            m_array.push_back(Json());
        }
    }
    return m_array.at(index);
}

const Json & Json::operator [] (int index) const
{
    return get(index);
}

Json & Json::operator [] (const char * key)
{
    string name = key;
    return (*this)[name];
}

Json & Json::operator [] (const string & key)
{
    if (type() == json_null)
    {
        m_type = json_object;
    }
    if (type() != json_object)
    {
        throw std::logic_error("function Json::operator [const string &] requires object value");
    }
    return m_object[key];
}

const Json & Json::operator [] (const char * key) const
{
    return get(key);
}

const Json & Json::operator [] (const string & key) const
{
    return get(key);
}

Json::operator bool()
{
    if (type() != json_bool)
    {
        throw std::logic_error("function Json::operator (bool) requires bool value");
    }
    return m_bool;
}

Json::operator int()
{
    if (type() != json_int)
    {
        throw std::logic_error("function Json::operator (int) requires int value");
    }
    return m_int;
}

Json::operator double()
{
    if (type() != json_double)
    {
        throw std::logic_error("function Json::operator (double) requires double value");
    }
    return m_double;
}

Json::operator string ()
{
    if (type() != json_string)
    {
        throw std::logic_error("function Json::operator (string) requires string value");
    }
    return m_string;
}

Json::operator string () const
{
    if (type() != json_string)
    {
        throw std::logic_error("function Json::operator (string) requires string value");
    }
    return m_string;
}

Json const & Json::null()
{
    static const Json null;
    return null;
}

void Json::parse(const string & str)
{
    Parser parser;
    parser.load(str);
    *this = parser.parse();
}

string Json::toString() const
{
    ostringstream os;
    switch (type())
    {
        case json_null:
            os << "null";
            break;
        case json_bool:
            if (m_bool)
            {
                os << "true";
            }
            else
            {
                os << "false";
            }
            break;
        case json_int:
            os << m_int;
            break;
        case json_double:
            os << m_double;
            break;
        case json_string:
            os << "\"" << m_string << "\"";
            break;
        case json_array:
            {
                os << "[";
                for (std::vector<Json>::const_iterator it = m_array.begin(); it != m_array.end(); it++)
                {
                    if (it != m_array.begin())
                    {
                        os << ",";
                    }
                    os << (*it).toString();
                }
                os << "]";
            }
            break;
        case json_object:
            {
                os << "{";
                for (std::map<string, Json>::const_iterator it = m_object.begin(); it != m_object.end(); it++)
                {
                    if (it != m_object.begin())
                    {
                        os << ",";
                    }
                    os << "\"" << it->first << "\":" << it->second.toString();
                }
                os << "}";
                return os.str();
            }
            break;
        default:
            break;
    }
    return os.str();
}