#pragma once

#include "libidk/Assert.hpp"
#include "libidk/Types.hpp"
#include "libidk/StdStr.hpp"

namespace idk
{
    template <size_t N>
    class RttiMetaData
    {
    private:
        char ascii[N];

    public:
        RttiMetaData(const char *s): ascii{}
        {
            IDK_ASSERT(idk_strlen(s) <= N-1, "[RttiMetaData] idstr \"{}\" exceeds max length {}", s, N);
            idk_strncpy(ascii, s, N-1);
        }

        constexpr size_t getMemLen() const { return N; }
        constexpr size_t getStrLen() const { return N-1; }

        const char &operator[](size_t idx) const
        {
            IDK_ASSERT(idx < N, "[RttiMetaData::operator[]] Invalid index {}", idx);
            return ascii[idx];
        }
    };


    struct IdkObject: private idk::Immobile
    {
        static inline const RttiMetaData<16> BASE_META = RttiMetaData<16>("IdkObject");
        const RttiMetaData<16> mBaseMeta = BASE_META;
        const RttiMetaData<16> mTypeMeta;

        IdkObject(const char *typeMeta): mTypeMeta(typeMeta)
        {

        }

        static bool isObject(const void *p)
        {
            return 0 == idk_strncmp((const char*)p, &BASE_META[0], BASE_META.getStrLen());
        }
    };

}
