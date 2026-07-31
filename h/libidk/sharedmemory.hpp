#pragma once

#include "libidk/types.hpp"

namespace idk::core
{
    template <typename T>
    class SharedMemory: public idk::NonCopyable, public idk::NonMovable
    {
    private:
        int file_;

    public:
        void  *base;
        size_t size;

        SharedMemory(const char *path);
        // ~SharedMemory();
    };
}


#ifdef __linux__
    #include <sys/mman.h>
    #include <sys/stat.h>
    #include <fcntl.h>
    #include <stdio.h>
    #include <unistd.h>
#else
    // include windows headers
#endif // __linux__


template <typename T>
idk::core::SharedMemory<T>::SharedMemory(const char *path)
{
#ifdef __linux__
    file_ = open(path, O_RDWR);
    struct stat st;
    fstat(file_, &st);

    base = mmap(NULL, st.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, file_, 0);
    size = st.st_size;

    if (base == MAP_FAILED)
    {
        perror("mmap failed");
        exit(1);
    }

#else
    // Windows stuff

#endif // __linux__
}

//     idk::core::SharedMemory::~SharedMemory()
//     {
//         munmap(base, size);
//         close(file_);
//     }


