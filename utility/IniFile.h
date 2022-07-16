#pragma once

#include <string.h>
#include <string>
#include <map>
using namespace std;

#include <sstream>
using std::ostringstream;

namespace yazi {
namespace utility {

class Value
{
public:
    Value();
    Value(bool value);
    Value(int value);
    Value(double value);
    Value(const string & value);
    ~Value();

    Value & operator = (bool value);
    Value & operator = (int value);
    Value & operator = (double value);
    Value & operator = (const string & value);
    
    operator bool();
    operator int();
    operator double();
    operator string();
    operator string() const;

private:
    string m_value;
};

class IniFile
{
public:
    IniFile();
    IniFile(const string &filename);
    ~IniFile();

    bool load(const string &filename);
    void save(const string &filename);
    void show();
    void clear();

    // read values in different formats
    Value & get(const string &section, const string &key);

    // set values in different formats
    void set(const string &section, const string &key, bool value);
    void set(const string &section, const string &key, int value);
    void set(const string &section, const string &key, double value);
    void set(const string &section, const string &key, const string &value);

    bool has(const string &section);
    bool has(const string &section, const string &key);

    void remove(const string &section);
    void remove(const string &section, const string &key);

    typedef std::map<string, Value> Section;
    Section & operator [] (const string & key)
    {
        return m_inifile[key];
    }

private:
    string trim(string s);

private:
    string m_filename;

    std::map<string, Section> m_inifile;
};

}}
