#pragma once

#include "libidk/cfgparser.hpp"
#include "libidk/dsa/Raii.hpp"

namespace idk
{
    class IEngine;
}

namespace idk::core
{
    class Service: public idk::Immobile
    {
    private:
        static constexpr size_t MAX_NAME_LENGTH = 64;
        const idk::IdType mTypeId;
        idk::CfgParser mCfgParser;
        idk::StringType<MAX_NAME_LENGTH> mName;
        RaiiFunc<void(Service&, const char**)> mRaii;
        static void raiiFunc_(idk::core::Service &srv, const char **name);

    protected:
        const CfgParser::TreeNode &mCfg;

    public:
        Service(const char *name, idk::IdType typeId);
        virtual ~Service() = default;
        virtual void update(IEngine*) = 0;
        virtual void shutdown(IEngine*) = 0;
        idk::IdType getTypeId() const { return mTypeId; }
        const char *getName() const { return &mName[0]; }
    };

    #define IDK_SERVICE_CTOR(DerivedType) idk::core::Service(#DerivedType, idk_typeid<DerivedType>())
}
