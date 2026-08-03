#pragma once

#include "libidk/Types.hpp"
#include "libidk/raii.hpp"
#include <atomic>

struct SDL_Window;


namespace idk
{
    uint64_t getSysTimeMs();
    uint64_t getSysTimeNs();

    enum class Status: uint8_t
    {
        Invalid = 0,
        Running,
        ShutdownInProgress,
        ShutdownComplete
    };

    class Platform: public idk::Immobile
    {
    public:
        using EventFunc = void (*)(Platform &plat, void *event, void *arg);

        Platform();
        ~Platform();

        bool running();
        void shutdown();
        void update();
        Status getStatus();
        Window &getWindow();

        bool addEventCallback(EventFunc, void *arg);

    private:
        RaiiFunc<void()>      mRaii;
        idk::platform::Window mWin;
        std::atomic<Status>   mStat;

        static constexpr size_t MAX_EVENT_FUNCS = 32;
        size_t    mNumEventFuncs;
        EventFunc mEventFuncs[MAX_EVENT_FUNCS];
        void     *mEventArgs[MAX_EVENT_FUNCS];

    };

}

