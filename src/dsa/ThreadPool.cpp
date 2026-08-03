#include "libidk/dsa/ThreadPool.hpp"

namespace idk::core
{
    ThreadPool::ThreadPool()
    :   mCurrTaskId(0),
        mQueueIdx(0)
    {
        for (size_t i=0; i<MAX_THREADS; i++)
        {
            size_t tid = i; //static_cast<IdType>(i);
            mThreads[i] = std::thread(ThreadPool::threadmain, this, tid);
            mThreads[i].detach();
        }
    }

    IdType ThreadPool::createTask(TaskFunc func, void *arg)
    {
        static ThreadPool::Task task;
        task.id = mCurrTaskId++;
        task.func = func;
        task.arg = arg;

        if (!mTaskQueue[mQueueIdx].push(task))
        {
            return ID_INVALID;
        }

        mQueueIdx = (mQueueIdx + 1) % MAX_THREADS;

        return task.id;
    }

    void ThreadPool::threadmain(ThreadPool *tp, size_t tid)
    {
        // static std::atomic<bool> running{true};
        auto &tq = tp->mTaskQueue[tid];

        // VLOG_INFO("[ThreadPool::threadmain {}] started thread {}", tid, tid);

        while (true)
        {
            static ThreadPool::Task task;
            if (!tq.pop(task))
            {
                continue;
            }

            if (task.id == ID_INVALID)
            {
                // VLOG_WARN("[ThreadPool::threadmain {}] task id == ID_INVALID", tid);
                continue;
            }

            task.func(task.arg);
        }
    }
}
