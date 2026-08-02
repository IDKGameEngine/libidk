#include "libidk/Config.hpp"
#include "libidk/New.hpp"

const idk::CfgParser::TreeNode &idk::config::get(const char *key)
{
    static idk::CfgParser *cfg = nullptr;
    if (cfg == nullptr)
    {
        cfg = idk::New<idk::CfgParser>();
        cfg->load("libidk.cfg");
    }
    return (*cfg)["libidk"][key];
}
