#include "libidk/SharedMemory.hpp"
#include "libidk/assert.hpp"
#include "libidk/log.hpp"
#include "libidk/stdstr.hpp"

#ifdef __linux__
    #include <sys/mman.h> // shm_open, mmap
    #include <sys/stat.h> // mode constants
    #include <unistd.h>   // ftruncate, close
    #include <fcntl.h>
    #include <cstring>
    #include <stdexcept>
    #include <cerrno>
#else

#endif


void *idk::SharedMemory::create(const char *name, size_t size)
{
    VLOG_INFO("[SharedMemory::create] A name={}", name);

    int fd = shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0666);
    if (fd < 0)
    {
        if (errno == EEXIST)
        {
            VLOG_WARN("Failure creating SharedMemory: region \"{}\" already exists", name);
        } 
        return nullptr;
        // VLOG_FATAL("Failure creating SharedMemory: {}", strerror(errno));
    }

    VLOG_INFO("[SharedMemory::create] B");
    if (ftruncate(fd, size) != 0)
    {
        close(fd);
        shm_unlink(name);
        return nullptr;
        // VLOG_FATAL("Failure creating SharedMemory: {}", strerror(errno));
    }

    VLOG_INFO("[SharedMemory::create] C");
    void *ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED)
    {
        VLOG_WARN("Failure on mmap");
        return nullptr;
    }

    VLOG_INFO("[SharedMemory::create] D");
    return ptr;
}


void *idk::SharedMemory::open(const char *name, size_t size)
{
    VLOG_INFO("[SharedMemory::open] A name={}", name);
    if (void *ptr = SharedMemory::create(name, size))
    {
        return ptr;
    }

    VLOG_INFO("[SharedMemory::open] B");
    int fd = shm_open(name, O_RDWR, 0666);
    IDK_ASSERT(fd>0, "Failure creating SharedMemory: {}", strerror(errno));

    VLOG_INFO("[SharedMemory::open] C");
    void *ptr = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED)
    {
        VLOG_WARN("Failure on mmap");
        return nullptr;
    }

    VLOG_INFO("[SharedMemory::open] D");
    return ptr;
}


// idk::SharedMemory::SharedMemory(int fd, size_t size, const char *name, bool hasOwnership)
// :   mFd(fd), mSize(size), mHasOwnership(hasOwnership)
// {
//     mBase = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
//     IDK_ASSERT(mBase != MAP_FAILED, "Failure on mmap");
// }


// idk::SharedMemory::~SharedMemory()
// {
//     if (mBase)         { munmap(mBase, mSize); }
//     if (mFd >= 0)      { close(mFd); }
//     if (mHasOwnership) { shm_unlink(mName); }
// }
