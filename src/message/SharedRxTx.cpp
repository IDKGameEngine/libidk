// #include "libidk/message/SharedRxTx.hpp"

// #include "libidk/assert.hpp"
// #include "libidk/log.hpp"
// #include "libidk/SharedMemory.hpp"
// #include "libidk/stdmem.hpp"
// #include "libidk/stdstr.hpp"

// #include <mutex>


// static constexpr size_t MAX_REGIONS = 32;
// struct SharedRxTxRegion
// {
//     static constexpr int64_t STATUS_AVAILABLE = 0;
//     static constexpr int64_t STATUS_UNAVAILABLE = 1;

//     std::mutex mutex;
//     int64_t status;
//     uint64_t tick;
//     uint8_t data[1500];

//     SharedRxTxRegion()
//     :   mutex(), status(STATUS_AVAILABLE), tick(0), data()
//     {

//     }
// };


// idk::SharedRxer::SharedRxer(const char *name, size_t size)
// :   mRegion(nullptr),
//     mTick(0)
// {
//     mRegion = idk::SharedMemory::open(name, size);
// }


// bool idk::SharedRxer::recvMsg(void *buf, size_t bufsz)
// {
//     auto *r = (SharedRxTxRegion*)mRegion;
//     std::lock_guard<std::mutex> lock(r->mutex);

//     if (mTick >= r->tick)
//     {
//         return false;
//     }

//     IDK_ASSERT(bufsz <= sizeof(r->data), "bufsz too large!");
//     idk_memcpy(buf, r->data, bufsz);
//     mTick = r->tick;

//     return true;
// }




// idk::SharedTxer::SharedTxer(const char *name, size_t size)
// :   mRegion(nullptr)
// {
//     mRegion = idk::SharedMemory::open(name, size);
// }


// bool idk::SharedTxer::sendMsg(const void *buf, size_t bufsz)
// {
//     auto *r = (SharedRxTxRegion*)mRegion;
//     std::lock_guard<std::mutex> lock(r->mutex);

//     IDK_ASSERT(bufsz <= sizeof(r->data), "bufsz too large!");
//     idk_memcpy(r->data, buf, bufsz);
//     r->tick += 1;

//     return true;
// }

