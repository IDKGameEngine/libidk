#pragma once

#include <cstddef>
#pragma once
#include <stddef.h>

namespace idk
{
    template <typename T>
    class LinkedList
    {
    private:
        T     *mRoot;
        size_t mSize;

        void makeLast(LinkedListNode *node)
        {
            if (node->next == nullptr)
            {
                return;
            }

            T *last = node;
            T *F = mRoot;
            T *L = node->prev;
            T *R = node->next;

            if (L) { L->next = R; }
            if (R) { R->prev = L; }

            while (last->next)
            {
                last = last->next;
            }

            last->next = node;
            node->prev = last;
            node->next = nullptr;

            if (F == node)
            {
                root = R;
            }
        }


    public:
        LinkedList(): mRoot(nullptr), mSize(0) {  };
        size_t size() const { return mSize; };

        void insert(T *node)
        {
            mSize++;

            if (mRoot == nullptr)
            {
                mRoot = node;
                return;
            }

            T *curr = mRoot;
            T *next = node;

            while (curr->next)
            {
                curr = curr->next;
            }

            curr->next = next;
            next->prev = curr;
        }

        void remove(T *node)
        {
            T *curr = mRoot;

            while (curr && (curr != node))
            {
                curr = curr->next;
            }

            if (curr == nullptr)
            {
                return;
            }

            makeLast(curr);
            curr->prev->next = nullptr;
            curr->prev = nullptr;
            
            mSize--;
        }

        struct iterator
        {
            T *curr;

            iterator(T *node): curr(node) {  };
            iterator(const iterator &it): curr(it.curr) {  };

            iterator &operator++()
            {
                curr = (T*)(curr->next);
                return *this;
            };

            iterator operator++(int)
            {
                return iterator(curr->next);
            };

            bool operator==(const iterator &rhs) { return curr == rhs.curr; };
            bool operator!=(const iterator &rhs) { return curr != rhs.curr; };
            T *&operator*() { return curr; };
        };

        iterator begin() { return iterator(mRoot); };
        iterator end()   { return iterator(nullptr); };

    };

}








// namespace idk
// {
//     template <typename T>
//     struct LinkedList
//     {
//     private:
//         T *head_;
//         size_t size_;

//     public:
//         LinkedList(): head_(nullptr) {  };

//         void insert(T*);
//         void remove(T*);
//         bool contains(T*);
//         T   *head() { return head_; }

//         size_t size() const { return size_; }
//         bool   empty() const { return size() == 0; }

//         struct iterator
//         {
//             T *node;
//             iterator(T *nd): node(nd) {  }
//             iterator(const iterator &I): node(I.node) {  }
//             iterator &operator++() { node = node->next_; return *this; };
//             iterator operator++(int) { return iterator(node->next_); };
//             bool operator==(const iterator &rhs ) { return node == rhs.node; };
//             bool operator!=(const iterator &rhs ) { return node != rhs.node; };
//             T *&operator*() { return node; };
//         };

//         iterator begin() { return iterator(head_); };
//         iterator end()   { return iterator(nullptr); };
//     };
// }



// template <typename T>
// void idk::dsa::LinkedList<T>::insert(T *node)
// {
//     node->next_ = head_;
//     head_ = node;
//     size_++;
// }


// template <typename T>
// void idk::dsa::LinkedList<T>::remove(T *node)
// {
//     if (!node || !head_)
//     {
//         return;
//     }

//     if (node == head_)
//     {
//         head_ = head_->next_;
//         return;
//     }

//     // Deletion somewhere in middle
//     // H -> A -> B -> C -> 0x00
//     // ---------------------------------------
//     T *prev = head_;
//     T *curr = head_->next_;

//     while (curr)
//     {
//         if (curr == node)
//         {
//             prev->next_ = node->next_;
//             node->next_ = nullptr;
//             return;
//         }
    
//         prev = curr;
//         curr = curr->next_;
//     }
// }


// template <typename T>
// bool idk::dsa::LinkedList<T>::contains(T *node)
// {
//     T *curr = head_;

//     while (curr)
//     {
//         if (curr == node)
//             return true;
//         curr = curr->next_;
//     }

//     return false;
// }
