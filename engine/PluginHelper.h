#pragma once

#include <dlfcn.h>

#include <string>
using std::string;

#include <map>

#include "Plugin.h"

namespace yazi {
namespace engine {

typedef Plugin * (*create_func)();
typedef void (*destroy_func)(Plugin * plugin);


class PluginHelper
{
public:
    PluginHelper();
    ~PluginHelper();

    void load(const string & plugin);
    void unload(const string & plugin);
    void * get(const string & plugin, const string & symbol);
    void show();

private:
    std::map<string, void *> m_plugins;
};

}}
