#include "libidk/Service.hpp"
#include "libidk/Engine.hpp"
#include "libidk/StdMem.hpp"
#include "libidk/StdStr.hpp"
#include "libidk/log.hpp"

static void ServiceRaiiFunc(const char **name)
{
    static char filepath[256];
    idk_memset(filepath, '\0', sizeof(filepath));
    snprintf(filepath, sizeof(filepath), "%s.cfg", *name);
    idk::IEngine::getCfgParser().load(filepath);
}

idk::core::Service::Service(const char *name, idk::IdType typeId)
:   mRaii(ServiceRaiiFunc, &name),
    mTypeId(typeId),
    mCfg(IEngine::getCfgParser()[name])
{
    idk_strncpy(&mName[0], name, MAX_NAME_LENGTH-1);
    mName[MAX_NAME_LENGTH - 1] = '\0';
}
