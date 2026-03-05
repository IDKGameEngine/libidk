#pragma once

#include "idkcfg/message.hpp"
#include "libidk/inplace_queue.hpp"
#include <memory>
#include <vector>


namespace idk
{
    class Service;
    class ServiceManager;

    class ServiceManager: public idk::NonCopyable
    {
    private:
        static idkid_t typeidx_;
        const idkid_t mTypeIdxBase;
        bool mEnabled;
        int _typeIdxToArrayIdx(idkid_t typeidx);

    protected:
        std::vector<idk::Service*> mServices;

    public:
        ServiceManager();
        void update();
        bool running() { return mEnabled; };
        void enable() { mEnabled = true; }
        void disable() { mEnabled = false; }

        template <typename T, typename... Args>
        T *registerService(Args...);

        template <typename T>
        T *getService();

        // void deleteService(idk::Service*);

        template <typename T>
        void srvMsgToSrv(idk::message, void*);
        void srvMsgToAll(idk::message, void*);

        template <typename T>
        void srvCmdToSrv(idk::command, void*);
        void srvCmdToAll(idk::command, void*);

    };


    class Service: public idk::NonCopyable
    {
    private:
        friend class ServiceManager;
        bool   mBrandOfSacrifice;
        idkid_t mTypeIdx;
        ServiceManager *mSrvManager;

    protected:
        /**
         * @return Number of services messaged
         */
        template <typename T>
        void srvMsgToSrv(idk::message msg, void *arg) { mSrvManager->srvMsgToSrv<T>(msg, arg); }
        void srvMsgToAll(idk::message msg, void *arg) { mSrvManager->srvMsgToAll(msg, arg); }

        template <typename T>
        void srvCmdToSrv(idk::command cmd, void *arg) { mSrvManager->srvCmdToSrv<T>(cmd, arg); }
        void srvCmdToAll(idk::command cmd, void *arg) { mSrvManager->srvCmdToAll(cmd, arg); }

        template <typename T>
        T *getService() { return mSrvManager->getService<T>(); }

    public:
        Service(): mBrandOfSacrifice(false) {  };
        virtual ~Service() = default;

        virtual void onUpdate() = 0;
        virtual void onEnable()   {  };
        virtual void onDisable()  {  };
        virtual void onMsgRecv(idk::message, void*) = 0;
        virtual void onCmdRecv(idk::command, void*) {  };

    };

}



template <typename T, typename... Args>
inline T *idk::ServiceManager::registerService(Args... args)
{
    static_assert(
        std::is_base_of_v<Service, T>,
        "T must be derivative of idk::Service"
    );

    if (getService<T>())
    {
        VLOG_ERROR("Attempted to register VaneTypeId=={} more than once\n", idk_typeid<T>());
        return nullptr;
    }

    T *srv = new T(args...);
       srv->mBrandOfSacrifice = false;
       srv->mTypeIdx    = srv_typeid<T>();
       srv->mSrvManager = this;
    mServices.push_back(srv);

    int idx_maybe = _typeIdxToArrayIdx(srv_typeid<T>()); 

    VLOG_INFO(
        "Registered service (TypeIdx, Idx, idx_maybe) == ({}, {}, {})",
        srv->mTypeIdx, mServices.size() - 1, idx_maybe
    );

    return srv;
}


template <typename T>
inline T *idk::ServiceManager::getService()
{
    static_assert(
        std::is_base_of_v<Service, T>,
        "T must be derivative of idk::Service"
    );

    const size_t desired_id = srv_typeid<T>();
    for (auto *srv: mServices)
        if (srv->mTypeIdx == desired_id)
            return static_cast<T*>(srv);
    return nullptr;
}


// inline void idk::ServiceManager::deleteService(Service *srv)
// {
//     srv->mBrandOfSacrifice = true;
// }


template <typename T>
inline void idk::ServiceManager::srvMsgToSrv(idk::message msg, void *arg)
{
    auto *srv = static_cast<Service*>(getService<T>());
    if (srv == nullptr)
        return;
    srv->onMsgRecv(msg, arg);
}


inline void idk::ServiceManager::srvMsgToAll(idk::message msg, void *arg)
{
    for (auto *srv: mServices)
    {
        srv->onMsgRecv(msg, arg);
    }
}



template <typename T>
inline void idk::ServiceManager::srvCmdToSrv(idk::command cmd, void *arg)
{
    auto *srv = static_cast<Service*>(getService<T>());
    if (srv == nullptr)
        return;
    if (cmd == command::SRV_SHUTDOWN)
        srv->mBrandOfSacrifice = true;
    else
        srv->onCmdRecv(cmd, arg);
}


inline void idk::ServiceManager::srvCmdToAll(idk::command cmd, void *arg)
{
    if (cmd == command::SRV_SHUTDOWN)
    {
        for (auto *srv: mServices)
            srv->mBrandOfSacrifice = true;
    }

    for (auto *srv: mServices)
    {
        srv->onCmdRecv(cmd, arg);
    }
}



