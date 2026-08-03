#pragma once

#include "libidk/Assert.hpp"
#include "libidk/log.hpp"
#include "libidk/Types.hpp"


namespace idk
{
    class Observer
    {
    private:
        Observer *mNext;

    public:
        Observer(): mNext(nullptr) {  };
        virtual ~Observer() = default;
        virtual void onNotify(void*) = 0;
    };

    class Observable
    {
    private:
        Observer *mObservers;

    public:
        Observable(): mObservers(nullptr) {  };
        void notify(void*);
    };
}
