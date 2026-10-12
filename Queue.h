#ifndef QUEUE_H
#define QUEUE_H

#include <cstddef>

template <typename T>
class Queue
{
private:
    T* data_;
    std::size_t capacity_;
    std::size_t front_;
    std::size_t count_;

public:
    explicit Queue(std::size_t capacity)
    {
        capacity_ = capacity;
        front_ = 0;
        count_ = 0;

        if (capacity_ > 0)
        {
            data_ = new T[capacity_];
        }
        else
        {
            data_ = nullptr;
        }
    }

    ~Queue()
    {
        delete[] data_;
    }

    bool enqueue(const T& value)
    {
        if (full())
        {
            return false;
        }

        std::size_t index = (front_ + count_) % capacity_;

        data_[index] = value;
        count_++;

        return true;
    }

    bool dequeue(T& value)
    {
        if (empty())
        {
            return false;
        }

        value = data_[front_];

        front_ = (front_ + 1) % capacity_;
        count_--;

        return true;
    }

    bool front(T& value) const
    {
        if (empty())
        {
            return false;
        }

        value = data_[front_];

        return true;
    }

    bool empty() const
    {
        return count_ == 0;
    }

    bool full() const
    {
        return count_ == capacity_;
    }

    std::size_t size() const
    {
        return count_;
    }

    std::size_t capacity() const
    {
        return capacity_;
    }
};

#endif