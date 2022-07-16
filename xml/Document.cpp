#include <ctype.h>
#include <stdexcept>
#include <fstream>
#include <sstream>

#include "Document.h"
using namespace yazi::xml;

Document::Document() : m_idx(0)
{
}

Document::~Document()
{
}

void Document::load_file(const string & filename)
{
    std::ifstream in(filename.c_str());
    ostringstream buf;
    char ch;
    while(in.get(ch))
    {
        buf.put(ch);
    }
    load_string(buf.str());
}

void Document::load_string(const string & str)
{
    m_str = str;
    m_idx = 0;
}

void Document::skip_white_spaces()
{
    while (m_str[m_idx] == ' ' || m_str[m_idx] == '\r' || m_str[m_idx] == '\n' || m_str[m_idx] == '\t')
        m_idx++;
}

Element Document::parse()
{
    skip_white_spaces();
    if (m_idx == m_str.size() || m_str[m_idx] == '\0')
    {
        throw std::logic_error("xml document is empty");
    }

    // parse xml declaration
    if (m_str[m_idx + 0] == '<' &&
        m_str[m_idx + 1] == '?' &&
        m_str[m_idx + 2] == 'x' &&
        m_str[m_idx + 3] == 'm' &&
        m_str[m_idx + 4] == 'l')
    {
        if (!parse_declaration())
        {
            throw std::logic_error("xml declaration is error");
        }
    }

    skip_white_spaces();

    // parse xml comment
    if (m_str[m_idx + 0] == '<' &&
        m_str[m_idx + 1] == '!' &&
        m_str[m_idx + 2] == '-' &&
        m_str[m_idx + 3] == '-')
    {
        if (!parse_comment())
        {
            throw std::logic_error("xml comment is error");
        }
    }

    skip_white_spaces();

    Element elem;
    string name;

    // parse xml element's
    if ((m_str[m_idx + 0] == '<') && (isalpha(m_str[m_idx + 1]) || m_str[m_idx + 1] == '_'))
    {
        m_idx++;

        // parse elem's name
        name = parse_element_name();
        elem.name(name);

        skip_white_spaces();

        while (m_str[m_idx] != '\0')
        {
            // empty tag
            if (m_str[m_idx + 0] == '/')
            {
                if (m_str[m_idx + 1] == '>')
                {
                    m_idx += 2;
                    return elem;
                }
                else
                {
                    throw std::logic_error("xml empty element is error");
                }
            }
            else if (m_str[m_idx + 0] == '<')
            {
                if (m_str[m_idx + 1] == '/')
                {
                    // find the end tag
                    string end_tag = "</" + name + ">";
                    size_t pos = m_str.find(end_tag, m_idx);
                    if (pos == std::string::npos)
                    {
                        throw std::logic_error("xml element " + name + " end tag not found");
                    }
                    if (pos == m_idx)
                    {
                        m_idx += end_tag.size();
                        return elem;
                    }
                }
                else if (m_str[m_idx + 0] == '<' &&
                         m_str[m_idx + 1] == '!' &&
                         m_str[m_idx + 2] == '-' &&
                         m_str[m_idx + 3] == '-')
                {
                    // parse xml comment
                    if (!parse_comment())
                    {
                        throw std::logic_error("xml comment is error");
                    }
                }
                else
                {
                    // parse child element
                    Element child = parse();
                    elem.append(child);
                }
            }
            else if (m_str[m_idx] == '>')
            {
                m_idx++;
                string text = parse_element_text();
                skip_white_spaces();
                if (text != "")
                {
                    elem.text(text);
                }
                else
                {
                    // parse child element
                    Element child = parse();
                    elem.append(child);
                }
            }
            else
            {
                // parse elem's attr
                string key = parse_element_attr_key();

                skip_white_spaces();

                if (m_str[m_idx] != '=')
                {
                    throw std::logic_error("xml element attr is error" + key);
                }
                m_idx++;
                string val = parse_element_attr_val();
                elem.attr(key, val);
            }

            skip_white_spaces();
        }
    }
    return elem;
}

bool Document::parse_declaration()
{
    size_t pos = m_str.find("?>", m_idx);
    if (pos == std::string::npos)
    {
        return false;
    }
    m_idx = pos + 2;
    return true;
}

bool Document::parse_comment()
{
    size_t pos = m_str.find("-->", m_idx);
    if (pos == std::string::npos)
    {
        return false;
    }
    m_idx = pos + 3;
    return true;
}

string Document::parse_element_name()
{
    skip_white_spaces();

    string out;
    if (isalpha(m_str[m_idx]) || (m_str[m_idx] == '_'))
    {
        out += m_str[m_idx];
        m_idx++;
        while ((m_str[m_idx] != '\0') &&
            (isalnum(m_str[m_idx]) || (m_str[m_idx] == '_') || (m_str[m_idx] == '-') || (m_str[m_idx] == ':')))
        {
            out += m_str[m_idx];
            m_idx++;
        }
    }
    return out;
}

string Document::parse_element_text()
{
    skip_white_spaces();

    string out;
    while (m_str[m_idx] != '<')
    {
        out += m_str[m_idx];
        m_idx++;
    }
    return out;
}

string Document::parse_element_attr_key()
{
    skip_white_spaces();

    string out;
    if (isalpha(m_str[m_idx]) || (m_str[m_idx] == '_'))
    {
        out += m_str[m_idx];
        m_idx++;
        while (isalnum(m_str[m_idx]) || (m_str[m_idx] == '_') || (m_str[m_idx] == '-') || (m_str[m_idx] == ':'))
        {
            out += m_str[m_idx];
            m_idx++;
        }
    }
    return out;
}

string Document::parse_element_attr_val()
{
    skip_white_spaces();

    if (m_str[m_idx] != '"')
    {
        throw std::logic_error("xml element attr value should be in double quotes");
    }
    m_idx++;

    string out;
    while (m_str[m_idx] != '"')
    {
        out += m_str[m_idx];
        m_idx++;
    }

    m_idx++;
    return out;
}