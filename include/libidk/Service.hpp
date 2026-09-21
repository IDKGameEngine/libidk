#pragma once

#include "libidk/Types.hpp"
#include "libidk/Assert.hpp"
#include "libidk/dsa/List.hpp"
#include "libidk/New.hpp"
#include <atomic>


namespace idk
{
    class EngineAPI;

    class Service;
    class ServiceManager;


    class Service: public idk::Immobile
    {
    public:
        Service() = default;
        virtual ~Service() = default;
        virtual void onInit(idk::EngineAPI&) = 0;
        virtual void onShutdown(idk::EngineAPI&) = 0;
        virtual void onUpdate(idk::EngineAPI&) {  };
        virtual void onFixedUpdate(idk::EngineAPI&) {  };
        virtual void onPreRender(idk::EngineAPI&) {  };
        virtual void onMidRender(idk::EngineAPI&) {  };
        virtual void onPostRender(idk::EngineAPI&) {  };
    };


    class ServiceManager: public idk::Immobile
    {
    private:
        idk::InplaceList<Service*, 32> mServices;

        template <typename MemberFunc, typename... Args>
        void service_dispatch(MemberFunc func, idk::EngineAPI& api, Args&&... args)
        {
            for (Service *service : mServices)
            {
                (service->*func)(api, args...);
            }
        }


    public:
        ServiceManager() = default;
        virtual ~ServiceManager() = default;

        void dispatchShutdown(EngineAPI &api)
        {
            size_t numServices = mServices.size();
            for (size_t i=0; i<numServices; i++)
            {
                mServices[numServices - i - 1]->onShutdown(api);
            }
        }

        void dispatchInit        (EngineAPI &api) { service_dispatch(&Service::onInit, api); }
        void dispatchUpdate      (EngineAPI &api) { service_dispatch(&Service::onUpdate, api); }
        void dispatchFixedUpdate (EngineAPI &api) { service_dispatch(&Service::onFixedUpdate, api); }
        void dispatchPreRender   (EngineAPI &api) { service_dispatch(&Service::onPreRender, api); }
        void dispatchMidRender   (EngineAPI &api) { service_dispatch(&Service::onMidRender, api); }
        void dispatchPostRender  (EngineAPI &api) { service_dispatch(&Service::onPostRender, api); }

        template <typename ServiceType>
        void addService(ServiceType *srv)
        {
            IDK_ASSERT(!mServices.full(), "[ServiceManager::addService] Cannot add service: At capacity.");
            mServices.push(srv);
        }

        template <typename ServiceType, typename... Args>
        void addService(Args&&... args)
        {
            IDK_ASSERT(!mServices.full(), "[ServiceManager::addService] Cannot add service: At capacity.");
            mServices.push(idk::New<ServiceType>(args...));
        }

        template <typename ServiceType>
        ServiceType *getService() noexcept
        {
            for (Service *srv: mServices)
            {
                if (ServiceType *ptr = dynamic_cast<ServiceType*>(srv))
                {
                    return ptr;
                }
            }
            return nullptr;
        }
    };
}
