#include "libidk/message/SharedRxTx.hpp"

#include "libidk/assert.hpp"
#include "libidk/log.hpp"
#include "libidk/stdmem.hpp"
#include "libidk/stdstr.hpp"

#include <mutex>

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


static constexpr size_t MAX_REGIONS = 32;
struct SharedRxTxRegion
{
    static constexpr int64_t STATUS_AVAILABLE = 0;
    static constexpr int64_t STATUS_UNAVAILABLE = 1;

    std::mutex mutex;
    int64_t status;
    uint64_t tick;
    uint8_t data[1500];

    SharedRxTxRegion()
    :   mutex(), status(STATUS_AVAILABLE), tick(0), data()
    {

    }
};


idk::SharedRxer::SharedRxer(const char *name, size_t size)
:   mRegion(nullptr),
    mTick(0)
{
    int fd = shm_open(name, O_CREAT | O_EXCL | O_RDONLY, 0666);
    if (fd < 0)
    {
        if (errno == EEXIST)
        {
            VLOG_WARN("Failure creating SharedMemory: region \"{}\" already exists", name);
        } 
        VLOG_FATAL("Failure creating SharedMemory: {}", strerror(errno));
    }

    VLOG_INFO("[SharedMemory::create] B");
    if (ftruncate(fd, size) != 0)
    {
        close(fd);
        shm_unlink(name);
        VLOG_FATAL("Failure creating SharedMemory: {}", strerror(errno));
    }

    VLOG_INFO("[SharedMemory::create] C");
    mRegion = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mRegion == MAP_FAILED)
    {
        VLOG_FATAL("Failure on mmap");
    }

    VLOG_INFO("[SharedMemory::create] D");
}


bool idk::SharedRxer::recvMsg(void *buf, size_t bufsz)
{
    auto *r = (SharedRxTxRegion*)mRegion;
    std::lock_guard<std::mutex> lock(r->mutex);

    if (mTick >= r->tick)
    {
        return false;
    }

    IDK_ASSERT(bufsz <= sizeof(r->data), "bufsz too large!");
    idk_memcpy(buf, r->data, bufsz);
    mTick = r->tick;

    return true;
}




idk::SharedTxer::SharedTxer(const char *name, size_t size)
:   mRegion(nullptr)
{
    int fd = shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0666);
    if (fd < 0)
    {
        if (errno == EEXIST)
        {
            VLOG_WARN("Failure creating SharedMemory: region \"{}\" already exists", name);
        } 
        VLOG_FATAL("Failure creating SharedMemory: {}", strerror(errno));
    }

    VLOG_INFO("[SharedMemory::create] B");
    if (ftruncate(fd, size) != 0)
    {
        close(fd);
        shm_unlink(name);
        VLOG_FATAL("Failure creating SharedMemory: {}", strerror(errno));
    }

    VLOG_INFO("[SharedMemory::create] C");
    mRegion = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mRegion == MAP_FAILED)
    {
        VLOG_FATAL("Failure on mmap");
    }

    VLOG_INFO("[SharedMemory::create] D");
}


bool idk::SharedTxer::sendMsg(const void *buf, size_t bufsz)
{
    auto *r = (SharedRxTxRegion*)mRegion;
    std::lock_guard<std::mutex> lock(r->mutex);

    IDK_ASSERT(bufsz <= sizeof(r->data), "bufsz too large!");
    idk_memcpy(r->data, buf, bufsz);
    r->tick += 1;

    return true;
}

