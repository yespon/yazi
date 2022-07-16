#pragma once

#include <string>
using std::string;

#include <vector>
#include <map>


namespace yazi {
namespace json {

class Json
{
public:
    enum Type
    {
        json_null = 0,  ///< 'null' value
        json_bool,      ///< bool value
        json_int,       ///< integer value
        json_double,    ///< double value
        json_string,    ///< UTF-8 string value
        json_array,     ///< array value (ordered list)
        json_object     ///< object value (collection of name/value pairs).
    };

    Json();
    Json(Type type);
    Json(bool value);
    Json(int value);
    Json(double value);
    Json(const char * value);
    Json(const string & value);
    Json(const Json & other);
    ~Json();

    Type type() const;

    bool isNull() const;
    bool isBool() const;
    bool isInt() const;
    bool isDouble() const;
    bool isString() const;
    bool isArray() const;
    bool isObject() const;

    bool asBool() const;
    int asInt() const;
    double asDouble() const;
    string asString() const;

    void copy(const Json & other);

    // number of values in array or object
    int size() const;

    
    // return true if empty array, empty object, or null, otherwise, false.
    bool empty() const;

    void clear();

    bool remove(int index);
    bool remove(const char * key);
    bool remove(const string & key);

    bool has(int index);
    bool has(const char * key);
    bool has(const string & key);

    const Json & get(int index) const;
    const Json & get(const char * key) const;
    const Json & get(const string & key) const;

    void set(const Json & other);
    void set(bool value);
    void set(int value);
    void set(double value);
    void set(const string & value);
    void set(const char * value);

    // Append value to array at the end.
    // Equivalent to m_array[m_array.size()] = value;
    Json & append(const Json & value);

    Json & operator = (const Json & other);
    Json & operator = (bool value);
    Json & operator = (int value);
    Json & operator = (double value);
    Json & operator = (const string & value);
    Json & operator = (const char * value);

    bool operator == (const Json & other);
    bool operator == (bool value);
    bool operator == (int value);
    bool operator == (double value);
    bool operator == (const string & value);
    bool operator == (const char * value);

    bool operator != (const Json & other);
    bool operator != (bool value);
    bool operator != (int value);
    bool operator != (double value);
    bool operator != (const string & value);
    bool operator != (const char * value);

    // Access an array element (zero based index)
    Json & operator [] (int index);
    // Access an array element (zero based index), returns null if there is no member with that name.
    const Json & operator [] (int index) const;

    // Access an object value by name, create a null member if it does not exist.
    Json & operator [] (const char * key);
    // Access an object value by name, returns null if there is no member with that name.
    const Json & operator [] (const char * key) const;

    // Access an object value by name, create a null member if it does not exist.
    Json & operator [] (const string & key);
    // Access an object value by name, returns null if there is no member with that name.
    const Json & operator [] (const string & key) const;

    operator bool();
    operator int();
    operator double();
    operator string();
    operator string() const;

    // static
    static Json const & null();

    void parse(const string & str);
    string toString() const;

    typedef std::vector<Json>::iterator iterator;
    typedef std::vector<Json>::const_iterator const_iterator;

    iterator begin()
    {
        return m_array.begin();
    }

    iterator end()
    {
        return m_array.end();
    }

    const_iterator begin() const
    {
        return m_array.begin();
    }

    const_iterator end() const
    {
        return m_array.end();
    }

private:
    Type m_type;

    int m_int;
    double m_double;
    bool m_bool;
    std::string m_string;
    std::vector<Json> m_array;
    std::map<string, Json> m_object;
};

}}