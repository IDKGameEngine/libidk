#pragma once

#include "IPlatform.hpp"


class idk::IPlatformVideo: public IdkObject
{
public:
    IPlatformVideo(): IdkObject("IPlatformVideo") {  }

    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;

private:

};
