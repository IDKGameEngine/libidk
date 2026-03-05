#pragma once

#define IDK_DEBUG


#ifdef IDK_DEBUG
    namespace idk::detail
    {
        void vnassert(const char *file, int line, bool cond, const char *condstr, const char *msg);
    }

    #define IDK_ASSERT(Cond, ...) idk::detail::vnassert(__FILE__, __LINE__, Cond, #Cond, std::format(__VA_ARGS__).c_str())
    // #define IDK_ASSERT(...) idk::detail::vnassert(cond, __FILE__, __LINE__, std::format(__VA_ARGS__).c_str())

#else
    #define IDK_ASSERT()

#endif
