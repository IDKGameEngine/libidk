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
        virtual void onUpdate(idk::EngineAPI&) = 0;
        virtual void onEvent(idk::EngineAPI&, const void*) = 0;
    };


    class ServiceManager: public idk::Immobile
    {
    private:
        idk::InplaceList<Service*, 32> mServices;

    public:
        ServiceManager() = default;
        virtual ~ServiceManager() = default;

        void initServices(idk::EngineAPI &api)
        {
            for (Service *srv: mServices)
            {
                srv->onInit(api);
            }
        }

        void shutdownServices(idk::EngineAPI &api)
        {
            size_t numServices = mServices.size();
            for (size_t i=0; i<numServices; i++)
            {
                mServices[numServices - i - 1]->onShutdown(api);
            }
        }

        void updateServices(idk::EngineAPI &api)
        {
            for (Service *srv: mServices)
            {
                srv->onUpdate(api);
            }
        }

        void broadcastEvent(idk::EngineAPI &api, const void *event)
        {
            for (Service *srv: mServices)
            {
                srv->onEvent(api, event);
            }
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
            for (Service *Service: mServices)
            {
                if (ServiceType *ptr = dynamic_cast<ServiceType*>(Service))
                {
                    return ptr;
                }
            }
            return nullptr;
        }
    };
}
