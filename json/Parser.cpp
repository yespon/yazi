#include <string.h>
#include <stdexcept>
#include <cstdlib>
#include "Parser.h"

using namespace yazi::json;

Parser::Parser() : m_idx(0)
{

}

Parser::~Parser()
{

}

void Parser::load(const string & str)
{
    m_str = str;
    m_idx = 0;
}

void Parser::skip_white_spaces()
{
    while (m_str[m_idx] == ' ' || m_str[m_idx] == '\r' || m_str[m_idx] == '\n' || m_str[m_idx] == '\t')
        m_idx++;
}

char Parser::get_next_token()
{
    skip_white_spaces();
    if (m_idx == m_str.size())
        throw std::logic_error("unexpected end of input");
    return m_str[m_idx++];
}

Json Parser::parse()
{
    char ch = get_next_token();

    if (ch == 'n')
    {
        m_idx--;
        return parse_null();
    }

    if (ch == 't' || ch == 'f')
    {
        m_idx--;
        return parse_bool();
    }

    if (ch == '-' || (ch >= '0' && ch <= '9'))
    {
        m_idx--;
        return parse_number();
    }
    
    if (ch == '"')
    {
        return parse_string();
    }

    if (ch == '[')
    {
        Json arr(Json::json_array);
        ch = get_next_token();
        if (ch == ']')
        {
            return arr;
        }
        while (true)
        {
            m_idx--;
            arr.append(parse());
            ch = get_next_token();
            if (ch == ']')
            {
                break;
            }
            if (ch != ',')
            {
                throw std::logic_error("expected ',' in array");
            }
            ch = get_next_token();
        }
        return arr;
    }

    
    if (ch == '{')
    {
        Json obj(Json::json_object);
        ch = get_next_token();
        if (ch == '}')
        {
            return obj;
        }

        while (true)
        {
            if (ch != '"')
            {
                throw std::logic_error("expected '\"' in object");
            }
            string key = parse_string();

            ch = get_next_token();
            if (ch != ':')
            {
                throw std::logic_error("expected ':' in object");
            }

            obj[key] = parse();

            ch = get_next_token();
            if (ch == '}')
            {
                break;
            }
            if (ch != ',')
            {
                throw std::logic_error("expected ',' in object");
            }
            ch = get_next_token();
        }
        return obj;
    }

    throw std::logic_error("unexpected character in parse json");
}

Json Parser::parse_null()
{
    if (m_str.compare(m_idx, 4, "null") == 0)
    {
        m_idx += 4;
        return Json();
    }
    throw std::logic_error("parse null error");
}

Json Parser::parse_bool()
{
    if (m_str.compare(m_idx, 4, "true") == 0)
    {
        m_idx += 4;
        return Json(true);
    }
    if (m_str.compare(m_idx, 5, "false") == 0)
    {
        m_idx += 5;
        return Json(false);
    }
    throw std::logic_error("parse bool error");
}

Json Parser::parse_number()
{
    size_t pos = m_idx;

    if (m_str[m_idx] == '-')
        m_idx++;

    // integer part
    if (m_str[m_idx] == '0')
    {
        m_idx++;
        if (in_range(m_str[m_idx], '0', '9'))
        {
            throw std::logic_error("leading 0s not permitted in numbers");
        }
    }
    else if (in_range(m_str[m_idx], '1', '9'))
    {
        m_idx++;
        while (in_range(m_str[m_idx], '0', '9'))
        {
            m_idx++;
        }
    }
    else
    {
        throw std::logic_error("invalid character in number");
    }

    if (m_str[m_idx] != '.')
    {
        return std::atoi(m_str.c_str() + pos);
    }

    // decimal part
    if (m_str[m_idx] == '.')
    {
        m_idx++;
        if (!in_range(m_str[m_idx], '0', '9'))
        {
            throw std::logic_error("at least one digit required in fractional part");
        }
        while (in_range(m_str[m_idx], '0', '9'))
        {
            m_idx++;
        }
    }

    return std::atof(m_str.c_str() + pos);
}

Json Parser::parse_string()
{
    string out;
    while (true)
    {
        if (m_idx == m_str.size())
        {
            throw std::logic_error("unexpected end of input in string");
        }

        char ch = m_str[m_idx++];
        if (ch == '"')
        {
            return out;
        }

        // The usual case: non-escaped characters
        if (ch != '\\')
        {
            out += ch;
            continue;
        }

        // Handle escapes
        throw std::logic_error("not support escaped characters in string");
    }
}

