#pragma once

#include "Plugin.h"
using namespace yazi::engine;

namespace yazi {
namespace plugin {

class UserPlugin : public Plugin
{
public:
    UserPlugin();
    virtual ~UserPlugin();

    virtual bool run(Context & ctx);

};

DEFINE_PLUGIN(UserPlugin)

}}
