#include "Workflow.h"
#include "Singleton.h"
#include "Context.h"
#include "Plugin.h"
#include "PluginHelper.h"
#include "Logger.h"

using namespace yazi::utility;
using namespace yazi::engine;


Workflow::Workflow()
{
}

Workflow::~Workflow()
{
    for (map<string, Work *>::iterator it = m_works.begin(); it != m_works.end(); ++it)
    {
        delete it->second;
        it->second = NULL;
    }
    m_works.clear();
}

bool Workflow::load(const string &workinfo)
{
    Document doc;
    doc.load_file(workinfo);
    Element root = doc.parse();
    info("load workflow success: %s", workinfo.c_str());

    for (Element::iterator it = root.begin(); it != root.end(); it++)
    {
        const string & name = it->attr("name");
        const string & flag = it->attr("switch");
        Work * work = new Work();
        work->set_name(name);
        if (flag == "on")
        {
            work->set_switch(true);
        }
        else if (flag == "off")
        {
            work->set_switch(false);
        }
        else
        {
            error("plugin switch: %s is not supported: %s", name.c_str(), flag.c_str());
            return false;
        }
        if (!load_plugin(work, (*it)))
        {
            return false;
        }
        m_works[name] = work;
    }
    return true;
}

bool Workflow::run(const string &work, const string &input, string &output)
{
    std::map<string, Work *>::iterator it = m_works.find(work);
    if (it == m_works.end())
    {
        error("work: %s is not exist!", work.c_str());
        return false;
    }
    if (!it->second->get_switch())
    {
        error("work: %s is switch off!", work.c_str());
        return false;
    }

    Context ctx;
    ctx.ref<string>("input") = input;
    if (!it->second->run(ctx))
    {
        error("work: %s run error!", work.c_str());
        return false;
    }
    output = ctx.ref<string>("output");
    return true;
}

bool Workflow::load_plugin(Work* work, Element & elem)
{
    for (Element::iterator it = elem.begin(); it != elem.end(); it++)
    {
        if (it->name() != "plugin")
        {
            error("plugin %s elem: %s is not supported", it->attr("name").c_str(), it->name().c_str());
            return false;
        }
        const string & name = it->attr("name");
        create_func func = (create_func)Singleton<PluginHelper>::instance()->get(name, "create");
        Plugin *plugin = func();
        plugin->set_name(name);

        const string & flag = it->attr("switch");
        if (flag == "on")
            plugin->set_switch(true);
        else if (flag == "off")
            plugin->set_switch(false);
        else
        {
            error("plugin %s switch: %s is not supported", name.c_str(), flag.c_str());
            return false;
        }
        work->append(plugin);
    }
    return true;
}

