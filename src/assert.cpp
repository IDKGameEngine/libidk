#include "libidk/assert.hpp"
#include <cassert>
#include <cstdarg>
#include <cstdio>
#include <cstdlib>


void idk::detail::vnassert(const char *file, int line, bool cond, const char *condstr, const char *msg)
{
    if (cond == true)
    {
        return;
    }

    std::printf("-------- ASSERT FAIL --------\n");
    std::printf("Assert:  %s\n", condstr);
    std::printf("Message: %s\n", msg);
    std::printf("Source:  %s:%d\n", file, line);
    std::abort();
}

