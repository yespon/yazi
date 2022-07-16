#pragma once

#include <string>
using std::string;

#include <vector>
#include <map>

namespace yazi {
namespace xml {

class Element
{
public:
    Element();
    Element(const string & name);
    Element(const string & name, const string & text);
    ~Element();

    // get element's name
    const string & name() const;

    // set element's name
    void name(const string & name);

    // get element's text
    const string & text() const;

    // set element's text
    void text(const string & text);

    // get element's attr
    string & attr(const string & key);

    // set element's attr
    void attr(const string & key, const string & value);

    // get child element by index
    const Element & operator [] (int index) const;

    // get child element by name
    const Element & operator [] (const string & name) const;

    // append child element
    void append(const Element & child);

    // get numbers of the child
    int size();

    void clear();
    
    string toString() const;

    typedef std::vector<Element>::iterator iterator;
    typedef std::vector<Element>::const_iterator const_iterator;

    iterator begin()
    {
        return m_children.begin();
    }

    iterator end()
    {
        return m_children.end();
    }

    const_iterator begin() const
    {
        return m_children.begin();
    }

    const_iterator end() const
    {
        return m_children.end();
    }

    static Element const & null();

private:
    string m_name;
    string m_text;
    std::vector<Element> m_children;
    std::map<string, string> m_attrs;
};

}}
