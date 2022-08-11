#include "TestPlugin.h"
using namespace yazi::engine;
using namespace yazi::plugin;


TestPlugin::TestPlugin() : Plugin()
{

}

TestPlugin::~TestPlugin()
{

}

bool TestPlugin::run(Context & ctx)
{
    string & input = ctx.ref<string>("input");
    ctx.ref<string>("output") = input + " test plugin run!\n";
    return false;
}

