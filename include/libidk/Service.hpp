#pragma once

#include "libidk/Types.hpp"
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
        virtual void init(ServiceManager*) = 0;
        virtual void update(ServiceManager*) = 0;
    };

    class ServiceManager: public idk::Immobile
    {
    private:
        static constexpr size_t MAX_SERVICES = 32;
        std::atomic_bool mRunning;
        
    protected:
        idk::InplaceList<Service*, MAX_SERVICES> mServices;

        // void init_services()
        // {
        //     for (Service *srv: mServices)
        //     {
        //         srv->update(this);
        //     }
        // }
        // void update_services()
        // {
        //     for (Service *srv: mServices)
        //     {
        //         srv->update(this);
        //     }
        // }

    public:
        ServiceManager(): mRunning(true) {  };
        virtual ~ServiceManager() = default;

        template <typename ServiceType, typename... Args>
        void addService(Args&&... args)
        {
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
