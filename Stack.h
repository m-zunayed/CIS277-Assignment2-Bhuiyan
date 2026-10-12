#ifndef STACK_H
#define STACK_H

#include <cstddef>
#include <stdexcept>

template <typename T>
class Stack
{
private:
    T* data;
    size_t count;
    size_t capacity;

    void resize()
    {
        size_t newCapacity = capacity * 2;
        T* newData = new T[newCapacity];

        for (size_t i = 0; i < count; i++)
        {
            newData[i] = data[i];
        }

        delete[] data;

        data = newData;
        capacity = newCapacity;
    }

public:
    Stack()
    {
        capacity = 4;
        count = 0;
        data = new T[capacity];
    }

    ~Stack()
    {
        delete[] data;
    }

    void push(const T& value)
    {
        if (count == capacity)
        {
            resize();
        }

        data[count] = value;
        count++;
    }

    T pop()
    {
        if (empty())
        {
            throw std::out_of_range("Stack is empty");
        }

        count--;
        return data[count];
    }

    T& top()
    {
        if (empty())
        {
            throw std::out_of_range("Stack is empty");
        }

        return data[count - 1];
    }

    bool empty() const
    {
        return count == 0;
    }

    size_t size() const
    {
        return count;
    }
};

#endif