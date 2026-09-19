#pragma once

#include "libidk/cfgparser.hpp"
#include "libidk/dsa/Raii.hpp"

namespace idk::core
{
    class Service: public idk::Immobile
    {
    private:

    public:
        // Service() = default;
        virtual ~Service() = default;
        virtual void update() = 0;
    };
}
