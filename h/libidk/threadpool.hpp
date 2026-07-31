#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

#include "libidk/Types.hpp"
#include "libidk/metric.hpp"
#include "libidk/queue.hpp"

namespace idk::core
{
    class ThreadPool
    {
    public:
        static constexpr size_t MAX_THREADS = 4;
        using TaskFunc = void (*)(void *arg);

        struct Task
        {
            idk::IdType id;
            TaskFunc func;
            void *arg;
            Task() : id(ID_INVALID), func(nullptr), arg(nullptr) {  }
        };

        ThreadPool();
        IdType createTask(TaskFunc func, void *arg);


    private:
        IdType mCurrTaskId;
        size_t mQueueIdx;
        std::thread mThreads[MAX_THREADS];
        core::Queue<Task, 256/MAX_THREADS> mTaskQueue[MAX_THREADS];

        static void threadmain(ThreadPool *tp, size_t tid);
    };

}
