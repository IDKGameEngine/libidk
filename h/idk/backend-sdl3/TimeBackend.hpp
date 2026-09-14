#pragma once

#include "idk/backend/ITimeBackend.hpp"


namespace idk
{
    class TimeBackend: public idk::ITimeBackend
    {
    private:

    public:
        virtual uint64_t getSysTimeMs() final;
        virtual uint64_t getSysTimeNs() final;

    };
}

