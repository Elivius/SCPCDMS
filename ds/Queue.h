#pragma once
// Linked-list FIFO queue.

#include <iostream>

template <typename T>
class Queue {
private:
    struct Node {
        T     value;
        Node* next = nullptr;
        explicit Node(const T& v) : value(v) {}
    };

    Node* head_ = nullptr;
    Node* tail_ = nullptr;
    int   count_ = 0;

public:
    Queue() = default;
    ~Queue() { T discard; while (dequeue(discard)) {} }
    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    bool isEmpty() const { return head_ == nullptr; }
    int  size() const    { return count_; }

    void enqueue(const T& value)
    {
        Node* n = new Node(value);
        if (tail_ == nullptr) head_ = tail_ = n;
        else { tail_->next = n; tail_ = n; }
        ++count_;
    }

    // false when the queue is empty
    bool dequeue(T& out)
    {
        if (head_ == nullptr) return false;
        Node* n = head_;
        out = n->value;
        head_ = n->next;
        if (head_ == nullptr) tail_ = nullptr;
        delete n;
        --count_;
        return true;
    }

    // false when the queue is empty
    bool front(T& out) const
    {
        if (head_ == nullptr) return false;
        out = head_->value;
        return true;
    }

    // visit(const T&) from front to back
    template <typename Visitor>
    void forEach(Visitor visit) const
    {
        for (const Node* n = head_; n != nullptr; n = n->next) visit(n->value);
    }

    void display() const
    {
        forEach([](const T& v) { std::cout << v << " "; });
        std::cout << "\n";
    }
};
