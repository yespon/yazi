#include <iostream>
#include <fstream>
#include <algorithm>
#include "IniFile.h"

using namespace yazi::utility;

Value::Value()
{
}

Value::Value(bool value)
{
    *this = value;
}

Value::Value(int value)
{
    *this = value;
}

Value::Value(double value)
{
    *this = value;
}

Value::Value(const string & value) : m_value(value)
{
}

Value::~Value()
{
}

Value & Value::operator = (bool value)
{
    if (value)
        m_value = "true";
    else
        m_value = "false";
    return *this;
}

Value & Value::operator = (int value)
{
    ostringstream os;
    os << value;
    m_value = os.str();
    return *this;
}

Value & Value::operator = (double value)
{
    ostringstream os;
    os << value;
    m_value = os.str();
    return *this;
}

Value & Value::operator = (const string & value)
{
    m_value = value;
    return *this;
}

Value::operator bool()
{
    if (m_value == "true")
        return true;
    else if (m_value == "false")
        return false;
    return false;
}

Value::operator int()
{
    return std::atoi(m_value.c_str());
}

Value::operator double()
{
    return std::atof(m_value.c_str());
}

Value::operator string()
{
    return m_value;
}

Value::operator string() const
{
    return m_value;
}

IniFile::IniFile()
{
}

IniFile::IniFile(const string &filename)
{
    load(filename);
}

IniFile::~IniFile()
{
}

string IniFile::trim(string s)
{
    if (s.empty())
    {
        return s;
    }
    s.erase(0, s.find_first_not_of(" \r\n"));
    s.erase(s.find_last_not_of(" \r\n") + 1);
    return s;
}

bool IniFile::load(const string &filename)
{
    m_filename = filename;
    m_inifile.clear();

    string name;
    string line;
    // open the INI file for reading
    ifstream fin(filename.c_str());
    if (fin.fail())
    {
        printf("loading file failed: %s is not found.\n", m_filename.c_str());
        return false;
    }
    while (std::getline(fin, line))
    {
        line = trim(line);
        if ('[' == line[0]) // it is a section
        {
            int pos = line.find_first_of(']');
            if (-1 != pos)
            {
                name = trim(line.substr(1, pos - 1));
                m_inifile[name];
            }
        }
        else if ('#' == line[0]) // it is a comment
        {
            continue;
        }
        else // it is the "key=value" line
        {
            int pos = line.find_first_of('=');
            if (pos > 0)
            {
                //add new key to the last section in the storage
                string key = trim(line.substr(0, pos));
                string value = trim(line.substr(pos + 1, line.size() - pos - 1));
                std::map<string, Section>::iterator it = m_inifile.find(name);
                if (it == m_inifile.end())
                {
                    printf("parsing error: section=%s key=%s\n", name.c_str(), key.c_str());
                    return false;
                }
                m_inifile[name][key] = value;
            }
        }
    }
    return true;
}

void IniFile::save(const string &filename)
{
    //open the INI file for writing
    ofstream fout(filename.c_str());
    std::map<string, Section>::iterator it;
    for (it = m_inifile.begin(); it != m_inifile.end(); ++it)
    {
        //write section line
        fout << "[" << it->first << "]" << endl;
        for (Section::iterator iter = it->second.begin(); iter != it->second.end(); ++iter)
        {
            //write "key = value" line
            fout << iter->first << " = " << (string)iter->second << endl;
        }
        fout << endl;
    }
}

void IniFile::show()
{
    std::map<string, Section>::iterator it;
    for (it = m_inifile.begin(); it != m_inifile.end(); ++it)
    {
        //write section line
        cout << "[" << it->first << "]" << endl;
        Section::iterator iter;
        for (iter = it->second.begin(); iter != it->second.end(); ++iter)
        {
            //write "key = value" line
            cout << iter->first << " = " << (string)iter->second << endl;
        }
        cout << endl;
    }
}

void IniFile::clear()
{
    m_inifile.clear();
}

bool IniFile::has(const string &section)
{
    return (m_inifile.find(section) != m_inifile.end());
}

bool IniFile::has(const string &section, const string& key)
{
    std::map<string, Section>::iterator it = m_inifile.find(section);
    if (it != m_inifile.end())
    {
        return (it->second.find(key) != it->second.end());
    }
    return false;
}

Value & IniFile::get(const string &section, const string &key)
{
    return m_inifile[section][key];
}

void IniFile::set(const string &section, const string &key, bool value)
{
    m_inifile[section][key] = value;
}

void IniFile::set(const string &section, const string &key, int value)
{
    m_inifile[section][key] = value;
}

void IniFile::set(const string &section, const string &key, double value)
{
    m_inifile[section][key] = value;
}

void IniFile::set(const string &section, const string &key, const string &value)
{
    m_inifile[section][key] = value;
}

void IniFile::remove(const string &section)
{
    std::map<string, Section>::iterator it = m_inifile.find(section);
    if (it != m_inifile.end())
        m_inifile.erase(it);
}

void IniFile::remove(const string &section, const string &key)
{
    std::map<string, Section>::iterator it = m_inifile.find(section);
    if (it != m_inifile.end())
    {
        Section::iterator iter = it->second.find(key);
        if (iter != it->second.end())
            it->second.erase(iter);
    }
}

