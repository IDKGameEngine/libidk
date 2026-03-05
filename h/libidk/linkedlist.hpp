#pragma once

#include <cstddef>


namespace idk::dsa
{
    template <typename T>
    struct LinkedList
    {
    private:
        T *head_;
        size_t size_;

    public:
        LinkedList(): head_(nullptr) {  };

        void insert(T*);
        void remove(T*);
        bool contains(T*);
        T   *head() { return head_; }

        size_t size() const { return size_; }
        bool   empty() const { return size() == 0; }

        struct iterator
        {
            T *node;
            iterator(T *nd): node(nd) {  }
            iterator(const iterator &I): node(I.node) {  }
            iterator &operator++() { node = node->next_; return *this; };
            iterator operator++(int) { return iterator(node->next_); };
            bool operator==(const iterator &rhs ) { return node == rhs.node; };
            bool operator!=(const iterator &rhs ) { return node != rhs.node; };
            T *&operator*() { return node; };
        };

        iterator begin() { return iterator(head_); };
        iterator end()   { return iterator(nullptr); };
    };
}



template <typename T>
void idk::dsa::LinkedList<T>::insert(T *node)
{
    node->next_ = head_;
    head_ = node;
    size_++;
}


template <typename T>
void idk::dsa::LinkedList<T>::remove(T *node)
{
    if (!node || !head_)
    {
        return;
    }

    if (node == head_)
    {
        head_ = head_->next_;
        return;
    }

    // Deletion somewhere in middle
    // H -> A -> B -> C -> 0x00
    // ---------------------------------------
    T *prev = head_;
    T *curr = head_->next_;

    while (curr)
    {
        if (curr == node)
        {
            prev->next_ = node->next_;
            node->next_ = nullptr;
            return;
        }
    
        prev = curr;
        curr = curr->next_;
    }
}


template <typename T>
bool idk::dsa::LinkedList<T>::contains(T *node)
{
    T *curr = head_;

    while (curr)
    {
        if (curr == node)
            return true;
        curr = curr->next_;
    }

    return false;
}