#pragma once

#include "libidk/cfgparser.hpp"

namespace idk::config
{
    const idk::CfgParser::TreeNode &get(const char *key);
}
