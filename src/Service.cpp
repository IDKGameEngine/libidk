#include "libidk/Service.hpp"
#include "libidk/Engine.hpp"
#include "libidk/StdMem.hpp"
#include "libidk/StdStr.hpp"
#include "libidk/log.hpp"

void idk::core::Service::raiiFunc_(Service &srv, const char **name)
{
    idk_strncpy(&(srv.mName[0]), *name, MAX_NAME_LENGTH-1);
    srv.mName[MAX_NAME_LENGTH - 1] = '\0';

    static idk::StringType<MAX_NAME_LENGTH + sizeof(".cfg")> cfgpath;
    idk_memset(cfgpath, '\0', sizeof(cfgpath));
    snprintf(cfgpath, sizeof(cfgpath), "%s.cfg", *name);

    srv.mCfgParser.load(cfgpath);
}

idk::core::Service::Service(const char *name, idk::IdType typeId)
:   mTypeId(typeId),
    mCfgParser(),
    mRaii(Service::raiiFunc_, *this, &name),
    mCfg(mCfgParser[name])
{

}
