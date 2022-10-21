#include "System.h"
using namespace yazi::utility;

#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/resource.h>

#include "Logger.h"
#include "IniFile.h"
#include "Singleton.h"
using namespace yazi::utility;

#include "Workflow.h"
using namespace yazi::engine;


System::System()
{
}

System::~System()
{
}

void System::init()
{
    core_dump();

    m_root_path = get_root_path();

    const string & logdir = m_root_path + "/log";
    DIR * dp = opendir(logdir.c_str());
    if (dp == NULL)
    {
        mkdir(logdir.c_str(), 0755);
    }
    else
    {
        closedir(dp);
    }

    // init logger
    Logger::instance()->open(m_root_path + "/log/main.log");

    // init inifile
    IniFile * ini = Singleton<IniFile>::instance();
    ini->load(get_root_path() + "/config/main.ini");

    // init workflow
    Workflow * workflow = Singleton<Workflow>::instance();
    workflow->load(get_root_path() + "/config/workflow.xml");
}

void System::core_dump()
{
    // core dump
    struct rlimit x;
    int ret = getrlimit(RLIMIT_CORE, &x);
    x.rlim_cur = x.rlim_max;
    ret = setrlimit(RLIMIT_CORE, &x);

    ret = getrlimit(RLIMIT_DATA, &x);
    x.rlim_cur = 768000000;
    ret = setrlimit(RLIMIT_DATA, &x);
}

string System::get_root_path()
{
    if (m_root_path != "")
    {
        return m_root_path;
    }
    char path[1024];
    memset(path, 0, 1024);
    int cnt = readlink("/proc/self/exe", path, 1024);
    if (cnt < 0 || cnt >= 1024)
    {
        return "";
    }
    for (int i = cnt; i >= 0; --i)
    {
        if (path[i] == '/')
        {
            path[i] = '\0';
            break;
        }
    }
    return string(path);
}
