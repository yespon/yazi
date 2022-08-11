#include "Work.h"
#include "Singleton.h"
#include "PluginHelper.h"

using namespace yazi::utility;
using namespace yazi::engine;

Work::Work() : m_name(""), m_switch(false)
{
}

Work::Work(const string & name, bool flag) : m_name(name), m_switch(flag)
{
}

Work::~Work()
{
    for (vector<Plugin *>::iterator it = m_plugins.begin(); it != m_plugins.end(); ++it)
    {
        destroy_func func = (destroy_func)Singleton<PluginHelper>::instance()->get((*it)->get_name(), "destroy");
        func(dynamic_cast<Plugin *>(*it));
    }
}

void Work::append(Plugin* plugin)
{
    m_plugins.push_back(plugin);
}

void Work::set_name(const string & name)
{
    m_name = name;
}

const string & Work::get_name() const
{
    return m_name;
}

void Work::set_switch(bool flag)
{
    m_switch = flag;
}

bool Work::get_switch() const
{
    return m_switch;
}

bool Work::run(Context & ctx)
{
    for (std::vector<Plugin *>::iterator it = m_plugins.begin(); it != m_plugins.end(); it++)
    {
        if ((*it)->get_switch())
        {
            if (!(*it)->run(ctx)) {
                break;
            }
        }
    }

    return true;
}
