#pragma once

#include "libidk/Assert.hpp"
#include "libidk/Types.hpp"

namespace idk
{
    template <typename T>
    class Observer;

    template <typename T>
    class Observable;
}


template <typename T>
class idk::Observer: public idk::Immobile
{
private:
    friend class Observable<T>;
    Observable<T> *mOwnr = nullptr;
    Observer      *mNext = nullptr;

protected:
    Observer() = default;

public:
    virtual ~Observer()
    {
        if (mOwnr) { mOwnr->unsubscribe(this); }
    }
    virtual void onNotify(const T&) = 0;
};


template <typename T>
class idk::Observable
{
private:
    Observer<T> *mHead = nullptr;

public:
    ~Observable()
    {
        while (mHead)
        {
            Observer<T> *next = mHead->mNext;
            mHead->mOwnr = nullptr;
            mHead->mNext = nullptr;
            mHead = next;
        }
    }

    void subscribe(Observer<T> *node)
    {
        if (node == nullptr)
        {
            VLOG_WARN("[Observable::subscribe] node == nullptr");
            return;
        }
        if (node->mOwnr == this)
        {
            return;
        }
        if (node->mOwnr)
        {
            node->mOwnr->unsubscribe(node);
        }

        node->mOwnr = this;
        node->mNext = mHead;
        mHead = node;
    }

    void unsubscribe(Observer<T> *node)
    {
        if (node == nullptr)
        {
            VLOG_WARN("[Observable::unsubscribe] node == nullptr");
            return;
        }
        if (node->mOwnr != this)
        {
            return;
        }
        if (node == mHead)
        {
            mHead = node->mNext;
            node->mOwnr = nullptr;
            node->mNext = nullptr;
            return;
        }

        Observer<T> *curr = mHead;
        while (curr)
        {
            if (curr->mNext == node)
            {
                curr->mNext = node->mNext;
                node->mOwnr = nullptr;
                node->mNext = nullptr;
                return;
            }
            curr = curr->mNext;
        }
    }

    void notify(const T &data)
    {
        mMidNotify = true;
        Observer<T> *curr = mHead;
        while (curr)
        {
            Observer<T> *next = curr->mNext;
            curr->onNotify(data);
            curr = next;
        }
        mMidNotify = false;
    }
};
