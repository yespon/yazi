#pragma once

#include "Plugin.h"
using namespace yazi::engine;

namespace yazi {
namespace plugin {

class EchoPlugin : public Plugin
{
public:
    EchoPlugin();
    virtual ~EchoPlugin();

    virtual bool run(Context & ctx);

};

DEFINE_PLUGIN(EchoPlugin)

}}