#include "UserPlugin.h"
using namespace yazi::engine;
using namespace yazi::plugin;

#include "Json.h"
using namespace yazi::json;

UserPlugin::UserPlugin() : Plugin()
{

}

UserPlugin::~UserPlugin()
{

}

bool UserPlugin::run(Context & ctx)
{
    string & input = ctx.ref<string>("input");

    Json json;
    json.parse(input);

    ctx.ref<string>("output") += "hello, " + json["name"].asString() + " user plugin run!\n";
    return true;
}

