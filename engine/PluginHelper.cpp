#include "PluginHelper.h"
using namespace yazi::engine;

#include "Logger.h"
#include "System.h"
#include "Singleton.h"
using namespace yazi::utility;


PluginHelper::PluginHelper()
{
}

PluginHelper::~PluginHelper()
{
}

void PluginHelper::load(const string & plugin)
{
    if (plugin.empty())
    {
        error("load plugin failure: plugin is empty.");
        return;
    }
    if (m_plugins.find(plugin) != m_plugins.end())
    {
        debug("plugin is already exist.");
        return;
    }

    System * sys = Singleton<System>::instance();
    string filename = sys->get_root_path() + "/plugin/" + plugin;
    void *handle = dlopen(filename.c_str(), RTLD_GLOBAL | RTLD_LAZY);
    if (handle == NULL)
    {
        error("load plugin failure: %s", dlerror());
        return;
    }
    m_plugins[plugin] = handle;
}

void PluginHelper::unload(const string & plugin)
{
    if (plugin.empty())
    {
        error("unload plugin failure: plugin is empty.");
        return;
    }
    std::map<string, void *>::iterator it = m_plugins.find(plugin);
    if (it == m_plugins.end())
    {
        error("unload plugin failure: plugin is not exist.");
        return;
    }
    dlclose(it->second);
    m_plugins.erase(it);
}

void * PluginHelper::get(const string & plugin, const string & symbol)
{
    std::map<string, void *>::iterator it = m_plugins.find(plugin);
    if (it == m_plugins.end())
    {
        load(plugin);
        it = m_plugins.find(plugin);
        if (it == m_plugins.end())
        {
            error("load plugin failure: %s", plugin.c_str());
            return NULL;
        }
    }

    void *func = dlsym(it->second, symbol.c_str());
    if (func == NULL)
    {
        error("plugin: %s, undefined symbol: %s", plugin.c_str(), symbol.c_str());
    }
    return func;
}

void PluginHelper::show()
{
    for (std::map<string, void *>::iterator it = m_plugins.begin(); it != m_plugins.end(); ++it)
    {
        debug("plugin: name=%s handle=%x", it->first.c_str(), it->second);
    }
}
