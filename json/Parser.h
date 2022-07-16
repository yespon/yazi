#pragma once

#include <string>
using namespace std;

#include "Json.h"

namespace yazi {
namespace json {

class Parser
{
public:
    Parser();
    ~Parser();

    void load(const string & str);
    Json parse();

private:
    void skip_white_spaces();
    char get_next_token();

    Json parse_null();
    Json parse_bool();
    Json parse_number();
    Json parse_string();

    bool in_range(long x, long lower, long upper)
    {
        return (x >= lower && x <= upper);
    }

private:
    string m_str;
    size_t m_idx;
};

}}
