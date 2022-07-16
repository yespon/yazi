#pragma once

#include "Plugin.h"
using namespace yazi::engine;

namespace yazi {
namespace plugin {

class TestPlugin : public Plugin
{
public:
    TestPlugin();
    virtual ~TestPlugin();

    virtual bool run(Context & ctx);

};

DEFINE_PLUGIN(TestPlugin)

}}
