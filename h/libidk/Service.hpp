#pragma once

#include "libidk/cfgparser.hpp"
#include "libidk/raii.hpp"

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
        RaiiFunc<void(const char**)> mRaii;
        const idk::IdType mTypeId;
        char mName[MAX_NAME_LENGTH];

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


    class IPlatformService: public Service
    {
    public:
        using Service::Service;
        virtual void getWindowSize(int &w, int &h) = 0;
        virtual void swapWindow() = 0;
    };

}
