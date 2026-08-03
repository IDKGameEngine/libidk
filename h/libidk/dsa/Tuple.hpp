#pragma once

namespace idk
{
    template <typename T, typename U>
    struct Pair
    {
        T first_;
        U second_;
        // Pair(const T &first=T(), const U &second=U()): first_(first), second_(second) {  }
    };

    template <typename T, typename U>
    idk::Pair<T, U> makePair(const T &x, const U &y)
    {
        return idk::Pair<T, U>{x, y};
    }
}

