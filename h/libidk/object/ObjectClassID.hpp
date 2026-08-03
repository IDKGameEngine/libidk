#pragma once

#include "libidk/Assert.hpp"
#include "libidk/StdStr.hpp"

namespace idk
{
    template <size_t N>
    class ObjectClassID
    {
    private:
        char ascii[N];

    public:
        ObjectClassID(const char *s): ascii{}
        {
            IDK_ASSERT(idk_strlen(s) <= N-1, "[ObjectClassID] idstr \"{}\" exceeds max length {}", s, N);
            idk_strncpy(ascii, s, N-1);
        }

        constexpr size_t getMemLen() const { return N; }
        constexpr size_t getStrLen() const { return N-1; }

        const char &operator[](size_t idx) const
        {
            IDK_ASSERT(idx < N, "[ObjectClassId::operator[]] Invalid index {}", idx);
            return ascii[idx];
        }
    };
}
