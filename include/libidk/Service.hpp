#pragma once

#include "libidk/Types.hpp"
#include "libidk/Assert.hpp"
#include "libidk/dsa/List.hpp"
#include "libidk/New.hpp"
#include <atomic>


namespace idk
{
    class Service;
    class ServiceManager;


    class Service: public idk::Immobile
    {
    public:
        Service() = default;
        virtual ~Service() = default;
        virtual void onInit(ServiceManager*) = 0;
        virtual void onUpdate(ServiceManager*) = 0;
        virtual void onShutdown(ServiceManager*) = 0;
        virtual void onEvent(ServiceManager*, const void*) = 0;
    };


    class ServiceManager: public idk::Immobile
    {
    private:
        idk::InplaceList<Service*, 32> mServices;

    public:
        virtual ~ServiceManager() = default;

        void initServices()
        {
            for (Service *srv: mServices)
            {
                srv->onInit(this);
            }
        }

        void updateServices()
        {
            for (Service *srv: mServices)
            {
                srv->onUpdate(this);
            }
        }

        void shutdownServices()
        {
            for (Service *srv: mServices)
            {
                srv->onShutdown(this);
            }
        }

        void broadcastEvent(const void *event)
        {
            for (Service *srv: mServices)
            {
                srv->onEvent(this, event);
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
